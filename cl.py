fin = open('test1.pas','r')
fout = open('test.pas','w')
for i in range(10000):
    s=fin.readline()
    if s=="":
        break
    fout.write(s.split('\t',1)[1])