<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*![rhaster_logo.png](../../../../Desktop/rhaster_logo.png)** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links ![rhaster_logo.png](../../../../Desktop/rhaster_logo.png)are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![Unlicense License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/alessandromanzini/rhaster-engine">
    <img src="images/rhaster_logo.png" alt="Logo">
  </a>

<h3 align="center">Rhaster Engine</h3>

  <p align="center">
    A modern C++ 2D game engine
    <br />
    <a href="https://github.com/alessandromanzini/rhaster-engine"><strong>Explore the docs »</strong></a>
    <br />
    <!--<a href="https://github.com/othneildrew/Best-README-Template">View Demo</a> 
    &middot;-->
    <a href="https://github.com/alessandromanzini/rhaster-engine/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/othneildrew/Best-README-Template/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->

## Table of Contents

<ul>
<li>
  <a href="#about-the-project">About The Project</a>
  <ul>
    <li><a href="#built-with">Built With</a></li>
  </ul>
</li>
<li>
  <a href="#getting-started">Getting Started</a>
  <ul>
    <li><a href="#prerequisites">Prerequisites</a></li>
    <li><a href="#installation">Installation</a></li>
  </ul>
</li>
<li><a href="#usage">Usage</a></li>
<li><a href="#roadmap">Roadmap</a></li>
<li><a href="#contributing">Contributing</a></li>
<li><a href="#license">License</a></li>
<li><a href="#contact">Contact</a></li>
<li><a href="#acknowledgments">Acknowledgments</a></li>
</ul>



<!-- ABOUT THE PROJECT -->

## About The Project

This project started as my **Programming 4** submission, but quickly
got addicted at making this a tool that I would confidently cho-
ose over the alternatives in the market.
This is still quite a far vision, but nonetheless this has been one of
my favorite projects, teaching me much about **performant low
level coding** and designing **optimal interfaces** for other engineers
to build upon.

### Key Features

|  |  |  |
|---|---|---|
| **🔧 Modern C++23** <br/> Latest standards & best practices | **🌍 Cross-Platform** <br/> Windows, Linux & macOS | **⚡ ECS Architecture** <br/> Flexible component system |
| **🎮 Advanced Input** <br/> Modifiers, triggers & rebinding | **🔊 Parallel Audio** <br/> High-performance queue | **🏗️ Clean Design** <br/> Scoped services, no singletons |
| **📊 Scene Graph** <br/> Hierarchical entity parenting | **🚀 Performance-First** <br/> Optimized for speed | **👨‍💻 Engineer-Friendly** <br/> Optimal interfaces |

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

[![C++23][C++23]]()
[![Cmake][CMake]][CMake-url]
[![SDL2][SDL2]][SDL2-url]
[![GLM][GLM]][GLM-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->

## Getting Started

### Prerequisites

* **C++23 Compiler** — Visual Studio 2022+ · Xcode 14+ · GCC 13+ · Clang 16+ 
  * 📚 [Setup Guide](https://isocpp.org/get-started)


* **CMake 3.20 or higher**
    * Download from the [official CMake website](https://cmake.org/download/)
    * Or install via package manager:
      ```sh
      # macOS (using Homebrew)
      brew install cmake
      ```
      ```sh
      # Ubuntu/Debian
      sudo apt-get install cmake
      ```
      ```sh
      # Windows (using Chocolatey)
      choco install cmake
      ```

### Installation

1. Clone the repository
   ```sh
   git clone https://github.com/alessandromanzini/rhaster-engine.git
   cd rhaster-engine
   ```

2. Generate build files with CMake
   ```sh
   cmake -B build
   ```

3. Build the project
   ```sh
   cmake --build build
   ```

4. Run the test executable
   ```sh
   # macOS/Linux
   ./out/debug/rhaster-test
   ```

    ```sh
   # Windows
   .\out\debug\rhaster-test.exe
   ```

> **Note:** Dependencies (GLM and SDL2) are automatically fetched during the CMake configuration step.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES 
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>-->



<!-- ROADMAP 
## Roadmap

- [x] Add Changelog
- [x] Add back to top links
- [ ] Add Additional Templates w/ Examples
- [ ] Add "components" document to easily copy & paste sections of the readme
- [ ] Multi-language Support
    - [ ] Chinese
    - [ ] Spanish

See the [open issues](https://github.com/othneildrew/Best-README-Template/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>-->



<!-- CONTRIBUTING -->

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any
contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also
simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Top contributors:

<a href="https://github.com/alessandromanzini/rhaster-engine/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=alessandromanzini/rhaster-engine" alt="contrib.rocks image" />
</a>

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->

## License

Distributed under the **MIT License**. See `LICENSE.txt` for more information.

The MIT License **allows** you to:
* ✅ Use commercially
* ✅ Modify
* ✅ Distribute
* ✅ Private use

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->

## Contact

Alessandro Manzini - [@linkedin][linkedin-url] - a.a.manzini00@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->

## Acknowledgments

* [Beautiful C++ - Guy Davidson](), from which I learned about correct abstraction and clean code design.
* [Game Programming Patterns - Robert Nystrom](https://gameprogrammingpatterns.com), essential guide to understand the
  intricate engine design field.
* [Digital Arts and Entertainment](https://www.digitalartsandentertainment.be), for the guidance and teachings.
* [Jakub Fratçzak](https://github.com/Froncu), dear friend from whom I got inspired for some of this project ideas.
* [Readme Template](https://github.com/othneildrew/Best-README-Template/blob/main/BLANK_README.md), for this handy
  readme template.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/alessandromanzini/rhaster-engine.svg?style=for-the-badge

[contributors-url]: https://github.com/alessandromanzini/rhaster-engine/graphs/contributors

[forks-shield]: https://img.shields.io/github/forks/alessandromanzini/rhaster-engine.svg?style=for-the-badge

[forks-url]: https://github.com/alessandromanzini/rhaster-engine/network/members

[stars-shield]: https://img.shields.io/github/stars/alessandromanzini/rhaster-engine.svg?style=for-the-badge

[stars-url]: https://github.com/alessandromanzini/rhaster-engine/stargazers

[issues-shield]: https://img.shields.io/github/issues/alessandromanzini/rhaster-engine.svg?style=for-the-badge

[issues-url]: https://github.com/alessandromanzini/rhaster-engine/issues

[license-shield]: https://img.shields.io/github/license/alessandromanzini/rhaster-engine.svg?style=for-the-badge

[license-url]: https://github.com/alessandromanzini/rhaster-engine/blob/master/LICENSE.txt

[linkedin-url]: https://linkedin.com/in/alessandro-manzini-developer

[C++23]: https://img.shields.io/badge/C++23-00599C?style=for-the-badge&logo=C%2B%2B&logoColor=white

[CMake]: https://img.shields.io/badge/CMake-FFFFFF?style=for-the-badge&logo=cmake&logoColor=013765

[CMake-url]: https://cmake.org

[SDL2]: https://img.shields.io/badge/SDL2-29415b?style=for-the-badge

[SDL2-url]: https://www.libsdl.org

[GLM]: https://img.shields.io/badge/GLM-ff8001?style=for-the-badge

[GLM-url]: https://github.com/g-truc/glm