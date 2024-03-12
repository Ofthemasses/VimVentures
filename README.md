# Welcome to VimVentures!

An open-source game, VimVentures, is a space-themed odyssey to mastering vim! Currently in development, this game invites you to a unique adventure across the cosmos.

## What's VimVentures?

Imagine piloting a spaceship through the vast universe, but there's a twist: solar flares are tampering with your ship's source code! Your mission is to wield your vim skills to overcome these cosmic challenges and keep your ship on path to it's next destination.

## Why VimVentures?
I began my journey learning Vim at the end of 2022. I have learned much from the vim community and want to give back!

My goal is to create a fun, and accessible way to learn vim that will be useful for users of any proficiency. Here are some of the planned features:

**Fun and Functionality Combined**: This isn't just any game; it's a vim-learning experience set against the backdrop of an interstellar voyage. There will be upgrades to your ship at the various stations you stop by, and a story that will keep you engaged throughout your journey.

**Thrilling Time-Sensitive Missions**: Tackle urgent code repairs to keep your spaceship on course, adding a thrilling aspect to your learning.

**VentureTutor (Name is WIP)**: As you progress through missions, the game tracks actions you might have missed, aiming to enhance your vim efficiency for subsequent voyages. Whether you complete your mission or not, you'll receive a list of your most missed actions, complete with demonstrations of where an alternative action could have been more efficient. For example:
```
You pressed 'x' the most efficient command here would be 'diw'
```

**Under Construction:** Alongside this being a contribution to the vim community, this is also a learning project for me!

If you want to learn more about VimVentures, all resources and information for the project are available at the [VimVentures Wiki](https://finlay.toru.studio/#/vim-ventures).

## Installation
Before installing, VimVentures relies on the following dependencies:
- SDL2
- X11
- Alacritty
- Vim

To install, perform the following inside of the root directory:

```
cmake .
make
```

Then, the game will be compiled inside of the `bin/` directory.
```
./bin/VimVentures
```

## Contributing
The game is in its early stages. Contribution guidelines will evolve as the project matures towards a phase where building a collaborative community becomes feasible.

**Source Code**: Source code is under the AGPL license, please keep this in mind if you wish to contribute.

**Assets**: All assets, including images, music, and sound effects, should adhere to the Creative Commons Attribution-NonCommercial-ShareAlike (CC BY-NC-SA) License. Contributions will be credited to the game's documentation and credits.
