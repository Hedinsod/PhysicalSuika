## 🍎 What It Is

This is a clone of the popular [Suika Game](https://www.suika-game.com/).

While playing the original, I often found myself frustrated by situations where two fruits were lying close together but not merging.
I really wanted to shake the glass to force some interaction – but since that wasn’t part of the game, I decided to make my own version where it is.

Another goal was educational: I wanted to better understand game physics, so I wrote the physics system myself from scratch. That’s also where the name of the project comes from.

The project is still a work in progress.

## 🔧 How to Build

This project uses **Premake5** to generate project files.

1. Find premake in `External\premake\bin\` or download latrest release from repository: [Premake5](https://premake.github.io/)
2. Run `premake5 [vs2019|gmake2|...]` depending on your platform/toolchain.
3. Open or build the generated project using your preferred IDE or build system

Note: I use vs2022 and never tested it with anything else.

## 🎮 How to Play

- **Left / Right Arrow** – Move the hand
- **Spacebar** – Drop a fruit

Enjoy the chaos 🍉🍇🍎

## Roadmap

- Add textures;
- Add some UI;
- Add special effects on fruit merge;
- Remake rendering with dx12;
