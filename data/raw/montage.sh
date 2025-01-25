#!/usr/bin/env bash

magick montage -background transparent -tile 4x3 -geometry 128x128 run/haut/* ../bubble-express/animation/run_up.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/diago_haut/* ../bubble-express/animation/run_up_left.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/bas/* ../bubble-express/animation/run_down.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/droite_gauche/* ../bubble-express/animation/run_left.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/diago_bas/* ../bubble-express/animation/run_down_right.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/pause/* ../bubble-express/animation/idle.png
magick montage -background transparent -tile 3x1 -geometry 136x136 base_bulle/* ../bubble-express/producers.png

magick montage -background transparent -tile 6x4 -geometry 200x200 bleu_detruirelunivers/* ../bubble-express/animation/bubble_blue.png
magick montage -background transparent -tile 6x4 -geometry 200x200 rouge_detruirelunivers/* ../bubble-express/animation/bubble_red.png
magick montage -background transparent -tile 6x4 -geometry 200x200 verte_detruirelunivers/* ../bubble-express/animation/bubble_green.png
magick montage -background transparent -tile 6x4 -geometry 200x200 jaune_detruirelunivers/* ../bubble-express/animation/bubble_yellow.png
