#! /usr/bin/python
# -*- coding:utf-8 -*-
import sys
import codecs

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "Usage: %s tab-file" %sys.argv[0]
        sys.exit (1)

    ifile = codecs.open (sys.argv[1])
    ofile = codecs.open (sys.argv[1] + ".sed", "w")
    hfile = codecs.open (sys.argv[1] + ".h", "w")

    hfile.write ("#include <glib.h>\n\n")
    j = 0;
    data = "\nstatic gpointer _map[] = {\n"
    
    for line in ifile.readlines ():
        one, two = line.strip().split ('\t') 
        if one != two:
            line = 's/' + one + '/' + two + '/g\n'
            line = line.replace ('-', '')
            ofile.write (line)

            onel = "".join ([hex(ord(i)) for i in one.decode ('utf-8')])
            twol = "".join ([hex(ord(i)) for i in two.decode ('utf-8')])
            string = 'static gunichar* _tmp'+ unicode(j) + '[] = { L"' + onel + '", L"' + twol + '"};' + '\n'
            string = string.replace ('0x', '\u')
            string = string.replace ('\u2d', '-')
            hfile.write (string)
            data = '\t' + data + '_tmp' + unicode(j) + ',\n'
            j = j + 1

    data = data + '};'
    hfile.write (data)
    
    ifile.close ()
    ofile.close ()
    hfile.close ()
