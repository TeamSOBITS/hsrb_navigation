<a name="readme-top"></a>

[JA](README.md) | [EN](README.en.md)

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
<!-- [![License][license-shield]][license-url] -->

# Flex Nav

<details>
  <summary>Table of Contents</summary>
<ol>
<li><a href="#introduction">Introduction</a></li>
<li><a href="#getting-started">Getting Started</a>
<ul>
<li><a href="#prerequisites">Prerequisites</a></li>
<li><a href="#installation">Installation</a></li>
</ul>
</li>
<li><a href="#launch-and-usage">Launch and Usage</a></li>
<li><a href="#parameters">Parameters</a></li>
<li><a href="#milestones">Milestones</a></li>
<li><a href="#references">References</a></li>
</ol>
</details>

## Introduction

This package enables a robot to navigate by leveraging its head-mounted depth sensor and scanning its surroundings by moving its head.

It is a customization of the implementation of [this paper](https://www.mathnet.ru/php/archive.phtml?wshow=paper&jrnid=trspy&paperid=1021&option_lang=eng) for Sobits robots.

```sh
@article{negrete:2018,
author 		= {Marco Negrete and Jesus Savage and Luis Contreras},
title 		= {{A Motion-Planning System for a Domestic Service Robot}},
journal		= {{SPIIRAS Proceedings}},
volume		= {60},
number		= {5},
pages		= {5--38},
year		= {2018}
}
```

  - Authors of the paper
      * **Marco Negrete** - [BioRobotics UNAM](https://biorobotics.fi-p.unam.mx/)
      * **Luis Contreras** - [AIBot](http://aibot.jp/)

Currently, this package is compatible with the following robots:

  - [HSRB (real robot)](https://github.com/TeamSOBITS/hsrb_robot)
  - [HSR (Sim)](https://github.com/TeamSOBITS/hsr_sim_common)
  - [SOBIT EDU](https://github.com/TeamSOBITS/sobit_edu),
    [SOBIT MINI](https://github.com/TeamSOBITS/sobit_mini),
    [SOBIT PRO](https://github.com/TeamSOBITS/sobit_pro)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

This section explains how to set up this repository.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Prerequisites

Before you proceed with the installation, make sure you have the following environment set up.
| System  | Version |
| --- | --- |
| Ubuntu | 22.04 (Jammy Jellyfish) |
| ROS    | Humble Hawksbill |
| Python | 3.10 |

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Installation

1.  Navigate to the `src` folder in your ROS2 workspace.
      ```
      cd ~/colcon_ws/src/
      ````

3.  Clone this repository.
      ```sh
      git clone -b humble-devel https://github.com/TeamSOBITS/flex_nav.git
      ```

5.  Navigate into the repository.
      ```sh
      cd flex_nav/
      ```

7.  Compile the package.
      ```sh
      cd ~/colcon_ws/
      ```
      ```sh
      colcon build --symlink-install
      ```
      ```sh
      source ~/colcon_ws/install/setup.sh
      ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Launch and Usage

Before running, please confirm that [Sobits Navigation Stack](https://github.com/TeamSOBITS/sobits_navigation_stack) is installed.

> [!NOTE]
> When using SOBIT EDU, MINI, or PRO, please perform the following before launching the robot:
>
> 1.  Open `sobit_(robot_name)/sobit_(robot_name)_control/config/controllers.yaml`.
> 2.  Add `allow_partial_joints_goal: true` before `command_interfaces` within `joint_trajectory_controller`.

1.  Launch the robot and confirm that 3D point cloud data is being published.
2.  In [Sobits Navigation Stack](https://github.com/TeamSOBITS/sobits_navigation_stack), replace **robot_name** in `nav2.launch.py` with the name of the robot you are using, then run the following command.
      ```sh
       ros2 launch sobits_nav nav2.launch.py
      ```
4.  In [flex_nav.launch.py](launch/flex_nav.launch.py), replace **robot_name** with the name of the robot you are using, then run the following command.
      ```sh
        ros2 launch flex_nav flex_nav.launch.py
      ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Parameters

The following are parameters that can be configured for each robot in [param](param).

| Parameter Name | Description | 
| --- | --- | 
| base_link | The name of the robot's base link |
| head_trajectory_topic | The ROS topic name for publishing head trajectories |
| head_pan_joint_name | The name of the pan joint for left-right head movement |
| head_tilt_joint_name | The name of the tilt joint for up-down head movement |
| max_pan_limit | The maximum pan joint angle of the head (in radians) |
| min_pan_limit | The minimum pan joint angle of the head (in radians) |
| max_tilt_limit | The maximum tilt joint angle of the head (in radians) |
| min_tilt_limit | The minimum tilt joint angle of the head (in radians) |
| tilt_angle_navigating | The tilt angle (in radians) at which the head is fixed while the robot is navigating |

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Milestones

  - [ ] Compatibility with [SOBIT LIGHT
    ](https://github.com/TeamSOBITS/sobit_light)
  - [ ] Adding other features

Please check the Issues page for current bugs and feature requests.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## References

[1] The implementation of human-following functionality is based on the following master's thesis:

```
@article{becerra:2012,
author 		= {Marco Becerra-Pedraza and Jesus Savage},
title 		= {{Sistema de seguimiento de personas para un robot movil de servicio}},
journal		= {{UNAM}},
year		= {2012}
}
```

You can download this thesis from this [link](https://web.siia.unam.mx/siia-publico/v/include/modulo_productos/tesis.php%3Fid%3D858286) (click on "URL: Ver Tesis").

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/TeamSOBITS/flex_nav.svg?style=for-the-badge
[contributors-url]: https://github.com/TeamSOBITS/flex_nav/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/TeamSOBITS/flex_nav.svg?style=for-the-badge
[forks-url]: https://github.com/TeamSOBITS/flex_nav/network/members
[stars-shield]: https://img.shields.io/github/stars/TeamSOBITS/flex_nav.svg?style=for-the-badge
[stars-url]: https://github.com/TeamSOBITS/flex_nav/stargazers
[issues-shield]: https://img.shields.io/github/issues/TeamSOBITS/flex_nav.svg?style=for-the-badge
[issues-url]: https://github.com/TeamSOBITS/flex_nav/issues
[license-shield]: https://img.shields.io/github/license/TeamSOBITS/flex_nav.svg?style=for-the-badge
[license-url]: LICENSE
