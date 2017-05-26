build: clean notes.tex veta

veta:
	latexmk -pdf veta-questions/veta-questions

notes.tex:
	latexmk -pdf

watch-notes:
	latexmk -pdf -pvc notes/notes

watch-veta:
	latexmk -pdf -pvc veta-questions/veta-questions

clean:
	latexmk -c