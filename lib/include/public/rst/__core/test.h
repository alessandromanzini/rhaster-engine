// #pragma once
// #include <vector>
// #include <unordered_map>
// #include <typeindex>
// #include <memory>
// #include <functional>
// #include <cassert>
//
// // Entity type - just an ID
// using Entity = std::uint32_t;
// constexpr Entity NULL_ENTITY = 0;
//
// // Base class for component pools
// class BasePool {
// public:
//     virtual ~BasePool() = default;
//     virtual void remove(Entity entity) = 0;
//     virtual bool has(Entity entity) const = 0;
//     virtual void clear() = 0;
// };
//
// // ============================================================================
// // GROUP IMPLEMENTATION
// // ============================================================================
//
// // Group class for maintaining sorted collections of entities
// template<typename... Components>
// class Group : public BaseGroup {
// private:
//     Registry& registry;
//     std::vector<Entity> entities;
//     std::function<bool(Entity, Entity)> comparator;
//     bool sorted = false;
//
//     // Check if entity has all required components
//     bool matches(Entity entity) const {
//         return (registry.template has<Components>(entity) && ...);
//     }
//
//     void ensure_sorted() {
//         if (!sorted && comparator) {
//             std::sort(entities.begin(), entities.end(), comparator);
//             sorted = true;
//         }
//     }
//
// public:
//     explicit Group(Registry& reg) : registry(reg) {}
//
//     // Set custom comparator for sorting
//     template<typename Compare>
//     void set_comparator(Compare&& comp) {
//         comparator = std::forward<Compare>(comp);
//         sorted = false;
//     }
//
//     // Refresh the group by scanning all entities
//     void refresh() override {
//         entities.clear();
//         sorted = false;
//
//         // Find all entities with required components
//         // We'll iterate through the smallest component pool
//         if constexpr (sizeof...(Components) > 0) {
//             const ComponentPool<std::tuple_element_t<0, std::tuple<Components...>>>* smallest_pool =
//                 &registry.template assure_pool<std::tuple_element_t<0, std::tuple<Components...>>>();
//
//             ((([&](){
//                 auto& pool = registry.template assure_pool<Components>();
//                 if (pool.size() < smallest_pool->size()) {
//                     smallest_pool = &pool;
//                 }
//             })()), ...);
//
//             for (Entity entity : smallest_pool->entities()) {
//                 if (matches(entity)) {
//                     entities.push_back(entity);
//                 }
//             }
//         }
//
//         ensure_sorted();
//     }
//
//     // Called when component is added to entity
//     void on_construct(Entity entity) override {
//         if (matches(entity)) {
//             // Check if already in group
//             auto it = std::find(entities.begin(), entities.end(), entity);
//             if (it == entities.end()) {
//                 entities.push_back(entity);
//                 sorted = false;
//             }
//         }
//     }
//
//     // Called when component is removed from entity
//     void on_destroy(Entity entity) override {
//         auto it = std::find(entities.begin(), entities.end(), entity);
//         if (it != entities.end()) {
//             entities.erase(it);
//             // No need to mark unsorted since we only removed
//         }
//     }
//
//     void clear() override {
//         entities.clear();
//         sorted = false;
//     }
//
//     // Iterator support
//     auto begin() {
//         ensure_sorted();
//         return entities.begin();
//     }
//
//     auto end() {
//         ensure_sorted();
//         return entities.end();
//     }
//
//     auto begin() const {
//         return entities.begin();
//     }
//
//     auto end() const {
//         return entities.end();
//     }
//
//     // Size and access
//     std::size_t size() const { return entities.size(); }
//     bool empty() const { return entities.empty(); }
//
//     Entity operator[](std::size_t index) const {
//         return entities[index];
//     }
//
//     // Execute function for each entity in group
//     template<typename Func>
//     void each(Func&& func) {
//         ensure_sorted();
//         for (Entity entity : entities) {
//             if constexpr (std::is_invocable_v<Func, Entity, Components&...>) {
//                 func(entity, registry.template get<Components>(entity)...);
//             } else {
//                 func(registry.template get<Components>(entity)...);
//             }
//         }
//     }
//
//     // Get entity at specific index (useful for sorted groups)
//     Entity at(std::size_t index) {
//         ensure_sorted();
//         return entities.at(index);
//     }
//
//     // Sort the group manually
//     void sort() {
//         if (comparator) {
//             std::sort(entities.begin(), entities.end(), comparator);
//             sorted = true;
//         }
//     }
//
// // Sparse set for efficient entity-component mapping

//

//
// // View for iterating over entities with specific components
// template<typename... Components>
// class View {
// private:
//     Registry& registry;
//
//     // Find the smallest component pool for iteration
//     std::size_t smallest_size() const {
//         std::size_t min_size = std::numeric_limits<std::size_t>::max();
//         ((min_size = std::min(min_size, registry.assure_pool<Components>().size())), ...);
//         return min_size;
//     }
//
//     template<typename Component>
//     const ComponentPool<Component>& get_pool() const {
//         return registry.assure_pool<Component>();
//     }
//
// public:
//     explicit View(Registry& reg) : registry(reg) {}
//
//     // Iterator that yields entities having all required components
//     class Iterator {
//     private:
//         const View& view;
//         std::size_t index;
//         const std::vector<Entity>* entities;
//
//         void advance_to_valid() {
//             while (index < entities->size()) {
//                 Entity entity = (*entities)[index];
//                 if ((view.registry.template has<Components>(entity) && ...)) {
//                     break;
//                 }
//                 ++index;
//             }
//         }
//
//     public:
//         Iterator(const View& v, std::size_t idx, const std::vector<Entity>* ents)
//             : view(v), index(idx), entities(ents) {
//             advance_to_valid();
//         }
//
//         Entity operator*() const {
//             return (*entities)[index];
//         }
//
//         Iterator& operator++() {
//             ++index;
//             advance_to_valid();
//             return *this;
//         }
//
//         bool operator!=(const Iterator& other) const {
//             return index != other.index;
//         }
//     };
//
//     Iterator begin() const {
//         // Use the smallest pool for iteration efficiency
//         const auto* smallest_pool = &registry.template assure_pool<std::tuple_element_t<0, std::tuple<Components...>>>();
//         std::size_t min_size = smallest_pool->size();
//
//         ((([&](){
//             auto& pool = registry.template assure_pool<Components>();
//             if (pool.size() < min_size) {
//                 min_size = pool.size();
//                 smallest_pool = &pool;
//             }
//         })()), ...);
//
//         return Iterator(*this, 0, &smallest_pool->entities());
//     }
//
//     Iterator end() const {
//         const auto* smallest_pool = &registry.template assure_pool<std::tuple_element_t<0, std::tuple<Components...>>>();
//         std::size_t min_size = smallest_pool->size();
//
//         ((([&](){
//             auto& pool = registry.template assure_pool<Components>();
//             if (pool.size() < min_size) {
//                 min_size = pool.size();
//                 smallest_pool = &pool;
//             }
//         })()), ...);
//
//         return Iterator(*this, smallest_pool->size(), &smallest_pool->entities());
//     }
//
//     // Execute function for each entity in view
//     template<typename Func>
//     void each(Func&& func) {
//         for (Entity entity : *this) {
//             if constexpr (std::is_invocable_v<Func, Entity, Components&...>) {
//                 func(entity, registry.template get<Components>(entity)...);
//             } else {
//                 func(registry.template get<Components>(entity)...);
//             }
//         }
//     }
// };
//
// // Usage example and test
// #include <iostream>
//
// // Example components
// struct Position {
//     float x, y;
//     Position(float x = 0, float y = 0) : x(x), y(y) {}
// };
//
// struct Velocity {
//     float dx, dy;
//     Velocity(float dx = 0, float dy = 0) : dx(dx), dy(dy) {}
// };
//
// struct Health {
//     int hp;
//     Health(int hp = 100) : hp(hp) {}
// };
//
// // Example usage
// // Example usage with groups
// void example_usage_with_groups() {
//     Registry registry;
//
//     // Create entities
//     auto entity1 = registry.create();
//     auto entity2 = registry.create();
//     auto entity3 = registry.create();
//     auto entity4 = registry.create();
//
//     // Add components with different positions for sorting demo
//     registry.emplace<Position>(entity1, 30.0f, 20.0f);
//     registry.emplace<Velocity>(entity1, 1.0f, 0.5f);
//     registry.emplace<Health>(entity1, 100);
//
//     registry.emplace<Position>(entity2, 10.0f, 15.0f);  // Lower X
//     registry.emplace<Velocity>(entity2, -1.0f, 1.0f);
//     registry.emplace<Health>(entity2, 75);
//
//     registry.emplace<Position>(entity3, 50.0f, 5.0f);   // Highest X
//     registry.emplace<Health>(entity3, 50);
//
//     registry.emplace<Position>(entity4, 20.0f, 25.0f);
//     registry.emplace<Velocity>(entity4, 0.5f, -0.5f);
//
//     // Create unsorted group of entities with Position and Health
//     auto& health_group = registry.group<Position, Health>();
//     std::cout << "Health group size: " << health_group.size() << "\n";
//
//     health_group.each([](Entity entity, Position& pos, Health& health) {
//         std::cout << "Entity " << entity << " at (" << pos.x << ", " << pos.y
//                   << ") with " << health.hp << " HP\n";
//     });
//
//     // Create sorted group by X position (ascending)
//     auto& sorted_by_x = registry.group<Position>([&registry](Entity a, Entity b) {
//         return registry.get<Position>(a).x < registry.get<Position>(b).x;
//     });
//
//     std::cout << "\nEntities sorted by X position:\n";
//     for (Entity entity : sorted_by_x) {
//         auto& pos = registry.get<Position>(entity);
//         std::cout << "Entity " << entity << " at X: " << pos.x << "\n";
//     }
//
//     // Create movement group (Position + Velocity) sorted by velocity magnitude
//     auto& movement_group = registry.group<Position, Velocity>([&registry](Entity a, Entity b) {
//         const auto& vel_a = registry.get<Velocity>(a);
//         const auto& vel_b = registry.get<Velocity>(b);
//         float mag_a = vel_a.dx * vel_a.dx + vel_a.dy * vel_a.dy;
//         float mag_b = vel_b.dx * vel_b.dx + vel_b.dy * vel_b.dy;
//         return mag_a > mag_b; // Sort by velocity magnitude (descending)
//     });
//
//     std::cout << "\nMoving entities sorted by velocity magnitude:\n";
//     movement_group.each([](Entity entity, Position& pos, Velocity& vel) {
//         float magnitude = std::sqrt(vel.dx * vel.dx + vel.dy * vel.dy);
//         std::cout << "Entity " << entity << " velocity magnitude: " << magnitude << "\n";
//     });
//
//     // Add a new entity and see groups update automatically
//     std::cout << "\nAdding new entity...\n";
//     auto new_entity = registry.create();
//     registry.emplace<Position>(new_entity, 5.0f, 10.0f);  // Lowest X
//     registry.emplace<Health>(new_entity, 25);
//
//     std::cout << "Health group size after addition: " << health_group.size() << "\n";
//     std::cout << "Sorted group first entity (should be new one): " << sorted_by_x.at(0) << "\n";
// }
//
// void example_usage() {
//     Registry registry;
//
//     // Create entities
//     auto entity1 = registry.create();
//     auto entity2 = registry.create();
//     auto entity3 = registry.create();
//
//     // Add components
//     registry.emplace<Position>(entity1, 10.0f, 20.0f);
//     registry.emplace<Velocity>(entity1, 1.0f, 0.5f);
//     registry.emplace<Health>(entity1, 100);
//
//     registry.emplace<Position>(entity2, 5.0f, 15.0f);
//     registry.emplace<Velocity>(entity2, -1.0f, 1.0f);
//
//     registry.emplace<Position>(entity3, 0.0f, 0.0f);
//     registry.emplace<Health>(entity3, 50);
//
//     // Movement system - iterate over entities with Position and Velocity
//     auto movement_system = [&registry]() {
//         auto view = registry.view<Position, Velocity>();
//         view.each([](Position& pos, Velocity& vel) {
//             pos.x += vel.dx;
//             pos.y += vel.dy;
//         });
//     };
//
//     // Health system - iterate over entities with Health
//     auto health_system = [&registry]() {
//         auto view = registry.view<Health>();
//         view.each([](Entity entity, Health& health) {
//             if (health.hp <= 0) {
//                 std::cout << "Entity " << entity << " is dead!\n";
//             }
//         });
//     };
//
//     // Run systems
//     std::cout << "Running movement system...\n";
//     movement_system();
//
//     std::cout << "Running health system...\n";
//     health_system();
//
//     // Check component values
//     std::cout << "Entity " << entity1 << " position: "
//               << registry.get<Position>(entity1).x << ", "
//               << registry.get<Position>(entity1).y << "\n";
//
//     // Demo groups
//     std::cout << "\n--- Group Demo ---\n";
//     example_usage_with_groups();
// }