#!/usr/bin/env bash

magick montage -background transparent -tile 4x3 -geometry 128x128 run/haut/* ../bubble-express/animation/run_up.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/diago_haut/* ../bubble-express/animation/run_up_left.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/bas/* ../bubble-express/animation/run_down.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/droite_gauche/* ../bubble-express/animation/run_left.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/diago_bas/* ../bubble-express/animation/run_down_right.png
magick montage -background transparent -tile 4x3 -geometry 128x128 run/pause/* ../bubble-express/animation/idle.png
