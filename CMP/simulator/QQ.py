import os
import commands

result = ""
incs = 0
config =""
for root, dirs, files in os.walk("../Student_valid_testcase/"):
	for dir in dirs:
		print dir
		os.system('cp ../Student_valid_testcase/%s/dimage.bin dimage.bin'%(dir))
		os.system('cp ../Student_valid_testcase/%s/iimage.bin iimage.bin'%(dir))
		os.system('./golden_CMP %s'%(config))
		os.system('cp snapshot.rpt ../Student_valid_testcase/%s'%(dir))
		os.system('cp report.rpt ../Student_valid_testcase/%s'%(dir))

for root, dirs, files in os.walk("../Student_valid_testcase/"):
	for dir in dirs:
		print dir
		os.system('cp ../Student_valid_testcase/%s/dimage.bin dimage.bin'%(dir))
		os.system('cp ../Student_valid_testcase/%s/iimage.bin iimage.bin'%(dir))
		os.system('./CMP %s'%(config))
		status, result1 = commands.getstatusoutput('diff ../Student_valid_testcase/%s/snapshot.rpt snapshot.rpt'%(dir))
		status, result2 = commands.getstatusoutput('diff ../Student_valid_testcase/%s/report.rpt report.rpt'%(dir))
		if result1 != "":
			result += 'fail in ' + dir + ' test snapshot '
		if result2 !="":
			result += 'fail in' + dir + ' test report '
		if result1 !="" or result2!="":
			result += '\n'
			incs = incs+1
	break
if result == "":
	print 'pass all'
else:
	print result
	print  incs
