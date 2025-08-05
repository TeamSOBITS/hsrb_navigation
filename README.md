<a name="readme-top"></a>

[JA](README.md) | [EN](README.en.md)

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]

# hsrb_navigation

<!-- 目次 -->
<details>
  <summary>目次</summary>
  <ol>
    <li>
      <a href="#概要">概要</a>
    </li>
    <li>
      <a href="#セットアップ">セットアップ</a>
      <ul>
        <li><a href="#環境条件">環境条件</a></li>
        <li><a href="#インストール方法">インストール方法</a></li>
      </ul>
    </li>
    <li><a href="#実行操作方法">実行・操作方法</a></li>
    <li><a href="#マイルストーン">マイルストーン</a></li>
  </ol>
</details>

<!-- 概要 -->
# 概要
ナビゲーション時に首振りをすることで,ロボットの頭部にある深度センサを活用しながらナビゲーションできるパッケージです.

現在以下のロボットに対応しています.
- hsrb_robot
- hsr_sim


<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

<!-- セットアップ -->
## セットアップ
ここで，本レポジトリのセットアップ方法について説明します．

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

### 環境条件
まず，以下の環境を整えてから，次のインストール方法に進んでください．
| System  | Version |
| --- | --- |
| Ubuntu | 22.04 (Jammy Jellyfish) |
| ROS    | Humble Hawksbill |
| Python | 3.10 |

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>


### インストール方法
1. ROS2の`src`フォルダに移動します．
    ```sh
    cd ~/colcon_ws/src/
    ```

2. 本レポジトリをcloneします．
    ```sh
    git clone -b feature/multi_robot  https://github.com/TeamSOBITS/hsrb_navigation.git
    ```
3. レポジトリの中へ移動します．
    ```sh
    cd hsrb_navigation/
    ```
4. パッケージをコンパイルします．
    ```sh
    cd ~/colcon_ws/
    ```
    ```sh
    colcon build --symlink-install
    ```
    ```sh
    source ~/colcon_ws/install/setup.sh
    ```

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

<!-- 実行・操作方法 -->
## 実行・操作方法
実行する前に,[Sobits Navigation Stack](https://github.com/TeamSOBITS/sobits_navigation_stack)がインストールされているか確認してください.

1. ロボットを起動し,3次元点群が発行されているか確認する.
2. [Sobits Navigation Stack](https://github.com/TeamSOBITS/sobits_navigation_stack)の`nav2.launch.py`の**robot_name**を使用するロボット名に書き換えて,以下のコマンドを実行する.
    ```sh
    ros2 launch sobits_nav nav2.launch.py 
    ```
3. [nav2_head_controller.launch.py](nav2_head_controller/launch/nav2_head_controller.launch.py)の**robot_name**を使用するロボット名に書き換えて,以下のコマンドを実行する.
    ```sh
    ros2 launch nav2_head_controller nav2_head_controller.launch.py 
   ```
<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

<!-- マイルストーン -->
## マイルストーン
現時点のバグや新規機能の依頼を確認するためにIssueページ をご覧ください．

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/TeamSOBITS/hsrb_navigation.svg?style=for-the-badge
[contributors-url]: https://github.com/TeamSOBITS/hsrb_navigation/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/TeamSOBITS/hsrb_navigation.svg?style=for-the-badge
[forks-url]: https://github.com/TeamSOBITS/hsrb_navigation/network/members
[stars-shield]: https://img.shields.io/github/stars/TeamSOBITS/hsrb_navigation.svg?style=for-the-badge
[stars-url]: https://github.com/TeamSOBITS/hsrb_navigation/stargazers
[issues-shield]: https://img.shields.io/github/issues/TeamSOBITS/hsrb_navigation.svg?style=for-the-badge
[issues-url]: https://github.com/TeamSOBITS/hsrb_navigation/issues
[license-shield]: https://img.shields.io/github/license/TeamSOBITS/hsrb_navigation.svg?style=for-the-badge
[license-url]: LICENSE


# References

[1] The human follower implementation is based on this master thesis work:

```
@article{becerra:2012,
author 		= {Marco Becerra-Pedraza and Jesus Savage},
title 		= {{Sistema de seguimiento de personas para un robot movil de servicio}},
journal		= {{UNAM}},
year		= {2012}
}
```

You can download it at the following [link](https://web.siia.unam.mx/siia-publico/v/include/modulo_productos/tesis.php?id=858286) (click on *URL: Ver Tesis*).

# Authors

* **Marco Negrete** - [BioRobotics UNAM](https://biorobotics.fi-p.unam.mx/)
* **Luis Contreras** - [AIBot](http://aibot.jp/)
