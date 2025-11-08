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
    <img src="images/rhaster_logo.png" alt="Logo" style="filter: brightness(0) invert(1);">
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

[SDL2]: https://img.shields.io/badge/SDL2-29415b?style=for-the-badge&logo=data:image/png%2bxml;base64,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&logoColor=white

[SDL2-url]: https://www.libsdl.org

[GLM]: https://img.shields.io/badge/GLM-ff8001?style=for-the-badge&logo=data:image/png%2bxml;base64,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&logoColor=white

[GLM-url]: https://github.com/g-truc/glm