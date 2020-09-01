import glob,os
import reportlab.pdfgen
import reportlab.lib.pagesizes
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, PageBreak, Preformatted
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.rl_config import defaultPageSize


dirs = ["ChessEngine","ChessEngineLibrary","UnitTesting","ChessUI"]
files = []

for folder in dirs:
    currentFiles = []
    for filename in glob.glob(folder + '/*.cpp') + glob.glob(folder + '/*.h'):
        currentFiles += [filename]
    currentFiles.sort()
    files += currentFiles

files.remove("ChessEngine\\resource.h")

stylesheet=getSampleStyleSheet()
doc = SimpleDocTemplate('myfile.pdf')
stylesheet.add(ParagraphStyle(name="lineNumber", fontSize=5, leading=6))

story = []

for each in files:
    print(each)


for filename in files:
    with open(filename) as file:
        content = file.read()
        story.append(Paragraph(filename + "\n\n",stylesheet["h2"]))
        counter = 1
        for line in content.split("\n"):
            temp = "{0:0>3}:    ".format(counter) + line.replace("\t"," "*10) 
            story.append(Preformatted(temp,stylesheet["lineNumber"]))
            counter+= 1
        story.append(PageBreak())

doc.build(story)
print("done")

        
    

