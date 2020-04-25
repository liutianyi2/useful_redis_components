import r_ziplist
import time

# 406M
#ls = []
#for i in range(0, 5000000):
#    ls.append(i)
#    ls.append("name_%s" % i)
#print "done, %s" % len(ls)
#time.sleep(100)
#exit()


# 210M
zl = r_ziplist.new()
for i in range(0, 5000000):
    r_ziplist.append(zl, str(i))
    r_ziplist.append(zl, "name_%s" % i)
print "done"
# 51M
time.sleep(100)
exit()
cursor = r_ziplist.cursor(zl)
print cursor
while 1:
    cursor, info = r_ziplist.traverse(zl, cursor)
    if not cursor:
        break
    print cursor,info
#exit()

