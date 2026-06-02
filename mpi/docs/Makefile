ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
FACTORY=docker
DOCKER_OPTIONS=run -it --rm -v "$(ROOT_DIR)":"$(ROOT_DIR)":Z -w "$(ROOT_DIR)" ghcr.io/csc-training/slidefactory:3.4.3
#FACTORY=slidefactory.sif
OPTIONS=slides --pandoc-args='--strip-comments'

SRC=$(wildcard *.md)
HTML=$(patsubst %.md,%.html,$(SRC))
PDF=$(patsubst %.md,%.pdf,$(SRC))

.PHONY: html pdf clean

html: $(HTML)

pdf: $(PDF)

clean:
	-rm -f $(HTML) $(PDF)

%.html: %.md
	$(FACTORY) $(DOCKER_OPTIONS) $(OPTIONS) --format html $<

%.pdf: %.md
	$(FACTORY) $(DOCKER_OPTIONS) $(OPTIONS) --format pdf $<
