<a name="readme-top"></a>

[JA](README.md) | [EN](README.en.md)

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
<!-- [![License][license-shield]][license-url] -->

# Flex Nav

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
    <li><a href="#パラメータ">パラメータ</a></li>
    <li><a href="#マイルストーン">マイルストーン</a></li>
    <li><a href="#参考文献">参考文献</a></li>
    <li><a href="#論文の著者">論文の著者</a></li>
  </ol>
</details>

<!-- 概要 -->
## 概要
ナビゲーション時に首振りをすることで,ロボットの頭部にある深度センサを活用しながらナビゲーションできるパッケージです.

[この論文](https://www.mathnet.ru/php/archive.phtml?wshow=paper&jrnid=trspy&paperid=1021&option_lang=eng)
を実装したパッケージをSobits用にカスタマイズしたものです.

現在以下のロボットに対応しています.
- [HSRB(実機)](https://github.com/TeamSOBITS/hsrb_robot)
- [HSR(Sim)](https://github.com/TeamSOBITS/hsr_sim_common)
- [SOBIT EDU
](https://github.com/TeamSOBITS/sobit_edu)


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
    git clone -b feature/multi_robot  https://github.com/TeamSOBITS/flex_nav.git
    ```
3. レポジトリの中へ移動します．
    ```sh
    cd flex_nav/
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

> [!NOTE]
> SOBIT EDUで使用する場合，ロボット起動前に以下を実行してください．
> 1. ``sobit_edu/sobit_edu_control/config/controllers.yaml``を開く
> 2. ``joint_trajectory_controller``の中の``command_interfaces``の前に，``allow_partial_joints_goal: true``を追記する．

1. ロボットを起動し,3次元点群が発行されているか確認する.
2. [Sobits Navigation Stack](https://github.com/TeamSOBITS/sobits_navigation_stack)の`nav2.launch.py`の**robot_name**を使用するロボット名に書き換えて,以下のコマンドを実行する.
    ```sh
    ros2 launch sobits_nav nav2.launch.py 
    ```
3. [flex_nav.launch.py](launch/flex_nav.launch.py)の**robot_name**を使用するロボット名に書き換えて,以下のコマンドを実行する.
    ```sh
    ros2 launch flex_nav flex_nav.launch.py 
   ```

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

## パラメータ
以下は[param](param)でロボットごとに設定可能なパラメータです．

| パラメータ名 | 説明 | 
| --- | --- | 
| base_link | ロボットのベースリンク名 |
| head_trajectory_topic | 頭部の軌道を送信するためのROSトピック名 |
| head_pan_joint_name | 頭部を左右に動かすパン関節名|
| head_tilt_joint_name | 頭部を上下に動かすチルト関節名 |
| max_pan_limit | 頭部のパン関節の最大角度（ラジアン）|
| min_pan_limit | 頭部のパン関節の最小角度（ラジアン）|
| max_tilt_limit |頭部のチルト関節の最大角度（ラジアン） |
| min_tilt_limit | 頭部のチルト関節の最小角度（ラジアン）|
| tilt_angle_navigating | ロボットが移動中に頭部を固定する際のチルト角度（ラジアン）|

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>


<!-- マイルストーン -->
## マイルストーン
- [ ] [SOBIT PRO
](https://github.com/TeamSOBITS/sobit_pro), [SOBIT MINI
](https://github.com/TeamSOBITS/sobit_mini), [SOBIT LIGHT
](https://github.com/TeamSOBITS/sobit_light)への対応
- [ ] 他の機能の追加

現時点のバグや新規機能の依頼を確認するためにIssueページ をご覧ください．

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

## 参考文献

[1] 人間の追従機能の実装は、以下の修士論文に基づいています:

```
@article{becerra:2012,
author 		= {Marco Becerra-Pedraza and Jesus Savage},
title 		= {{Sistema de seguimiento de personas para un robot movil de servicio}},
journal		= {{UNAM}},
year		= {2012}
}
```
この論文は、こちらの[link](https://web.siia.unam.mx/siia-publico/v/include/modulo_productos/tesis.php?id=858286)からダウンロードできます（「URL: Ver Tesis」をクリックしてください）．

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

### 論文の著者

* **Marco Negrete** - [BioRobotics UNAM](https://biorobotics.fi-p.unam.mx/)
* **Luis Contreras** - [AIBot](http://aibot.jp/)

<p align="right">(<a href="#readme-top">上に戻る</a>)</p>

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
