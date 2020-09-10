ws = []
with open("GermanNouns.csv", "r") as f:
    for l in f.readlines():
        first = l.split(",")[0]
        first = first.replace("ä","ae")
        first = first.replace("ö","oe")
        first = first.replace("ü","ue")
        first = first.replace("ß","ss")
        first = first.replace("Ä","Ae")
        first = first.replace("Ö","Oe")
        first = first.replace("Ü","Ue")

        if first.isalpha():
            if 'Toponym' in l:
                ws.append(first + "," + "LOCATION")
            elif 'Nachname' in l or 'Vorname' in l:
                ws.append(first + "," + "NAME")
            else:
                ws.append(first + "," + "OTHERS")

with open("GermanNounsProper.csv", "w") as fw:
    fw.write('\n'.join(ws))
