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

- generation
  - remove isolated ground
    - make a search in the graph to find the biggest connected component
    - remove all tiles that are not in the biggest connecter component
  - cities: make a circle of moutains
- physics
  - make bubble more stable
  - performance: look at spatial hash: https://chipmunk-physics.net/release/ChipmunkLatest-Docs/#cpSpace-SpatialHash
- minimap
  - create minimap image from map (1 pixel per tile)
  - handle fog of war
- compass
- contracts
- end of game
- start scene
  - create world asynchronously
