![license](https://img.shields.io/github/license/h1-mod/iw7-mod.svg)
[![open bugs](https://img.shields.io/github/issues/h1-mod/iw7-mod/bug?label=bugs)](https://github.com/h1-mod/iw7-mod/issues?q=is%3Aissue+is%3Aopen+label%3Abug)
[![discord](https://img.shields.io/discord/945420505157083208?color=%237289DA&label=members&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/RzzXu5EVnh)
<!--
[![Build](https://github.com/h1-mod/h1-mod/workflows/Build/badge.svg)](https://github.com/h1-mod/h1-mod/actions)
[![Build status](https://ci.appveyor.com/api/projects/status/0sh80kdnsvm53rno?svg=true)](https://ci.appveyor.com/project/h1-mod/h1-mod)
-->

# IW7-Mod

<p align="center">
  <img src="assets/github/banner.png?raw=true" />
</p>

<br>

**NOTE**: You must legally own a copy of Call of Duty®: Infinite Warfare to run this mod. Cracked/pirated versions of the game are **NOT** supported. Grab a copy of the game from Steam: https://store.steampowered.com/app/292730/Call_of_Duty_Infinite_Warfare/

## Compile from source

- Clone the Git repo. Do NOT download it as ZIP, that won't work.
- Update the submodules and run `premake5 vs2022` or simply use the delivered `generate.bat`.
- Build via solution file in `build\iw7-mod.sln`.

### Premake arguments

| Argument                    | Description                                    |
|:----------------------------|:-----------------------------------------------|
| `--copy-to=PATH`            | Optional, copy the EXE to a custom folder after build, define the path here if wanted. |
| `--dev-build`               | Enable development builds of the client. |

## Credits

- [h1-mod](https://github.com/h1-mod/h1-mod) - codebase and research
- [h2-mod](https://github.com/fedddddd/h2-mod) - research (successor of MWR)
- [momo5502](https://github.com/momo5502) - Arxan/Steam research, former lead developer of [XLabsProject](https://github.com/XLabsProject)

## Disclaimer

This software has been created purely for the purposes of academic research. It is not intended to be used to attack other systems. Project maintainers are not responsible or liable for misuse of the software. Use responsibly.
