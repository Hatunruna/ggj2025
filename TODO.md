# TODO

## Graphics

- tileset: 3 tiles with 4 variations each time
  - ground
  - cliff
  - block
- sprites
  - hero (with animations)
  - producers: 4 variations (2x2)
  - bubble

## Code

- ui
  - bubbles are not scaled
- lore
  - rewrite
- bubbles
  - desync animations
- hero
  - render arrows in their own entities
  - render only one arrow (and remove it when arriving near target)
- physics
  - reset all physics
  - fix producer shape
  - performance: look at spatial hash: https://chipmunk-physics.net/release/ChipmunkLatest-Docs/#cpSpace-SpatialHash
- map
  - render normal moutains more frequently
  - render gates in their own entity
  - make ground in front of gates
- end of game
  - reset of game
  - score
