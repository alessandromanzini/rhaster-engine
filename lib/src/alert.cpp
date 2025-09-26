#include <rst/__diagnostic/__warren/alert.h>

#include <rst/__internal/diagnostic/log.h>


namespace rst
{
    static std::ostream* alert_stream{ &std::cerr };


    auto set_global_alert_stream( std::ostream& os ) noexcept -> void
    {
        alert_stream = &os;
    }


    auto internal::alert_impl( std::string_view const message ) noexcept -> void
    {
        *alert_stream << log_sig << " " << message << std::endl;
    }
}
