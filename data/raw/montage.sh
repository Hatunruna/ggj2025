#!/usr/bin/env bash

magick montage -background transparent -tile 4x3 -geometry 128x128 run/haut/* ../bubble-express/animation/run_up.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/diago_haut/* ../bubble-express/animation/run_up_left.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/bas/* ../bubble-express/animation/run_down.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/droite_gauche/* ../bubble-express/animation/run_left.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/diago_bas/* ../bubble-express/animation/run_down_right.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/pause/* ../bubble-express/animation/idle.png

magick montage -background transparent -tile 8x8 -geometry 200x200 bleu_detruirelunivers/* ../bubble-express/animation/bubble_blue.png
magick montage -background transparent -tile 8x8 -geometry 200x200 rouge_detruirelunivers/* ../bubble-express/animation/bubble_red.png
magick montage -background transparent -tile 8x8 -geometry 200x200 verte_detruirelunivers/* ../bubble-express/animation/bubble_green.png
magick montage -background transparent -tile 8x8 -geometry 200x200 jaune_detruirelunivers/* ../bubble-express/animation/bubble_yellow.png

magick montage -background transparent -tile 3x1 -geometry 136x136 base_bulle/* ../bubble-express/producers.png
magick montage -background '#3c96ee' -tile 20x2 -geometry 64x64 ground_tiles/* ../bubble-express/ground_tiles.png

magick montage -background transparent -tile 4x4 -geometry 512x512 trap/01_debut/* ../bubble-express/animation/trap_spawn.png
magick montage -background transparent -tile 8x8 -geometry 512x512 trap/02_mine_boucle/* ../bubble-express/animation/trap_trigger.png
magick montage -background transparent -tile 8x8 -geometry 512x512 trap/03_mine_tire/* ../bubble-express/animation/trap_grow.png
magick montage -background transparent -tile 8x8 -geometry 512x512 trap/04_fumee_boucle/* ../bubble-express/animation/trap_active.png
magick montage -background transparent -tile 8x8 -geometry 512x512 trap/05_fumee_fin/* ../bubble-express/animation/trap_rest.png
