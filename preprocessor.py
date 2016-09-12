def firstfn (filename):
	f= open(filename, 'r')
	f1= f.readlines()
	flag=False
	string=""
	for line in f1:
	  if "subject:" in line.lower():
	    string =string + line
	  if line=='\n' or flag==True:
	    string= string + line; flag=True
	

	"Remove Ascii"	
	import re
	string1= re.sub(r'[^\x00-\x7F]+','', string)

	"Lower case"
	string= string1.lower()



	"Replace non-alphanumeric with space"		

	s = re.sub('[^0-9a-z]+', ' ', string)


	"Tokenize"
	s1=s.split()

	"Removing only digits"
	s2=[]
	for w in s1:
		#if re.search('[a-zA-Z]', w)== None:
		if w.isdigit()==False and len(w)>1:
			s2.append(w)


	"Removing stopwords"	
	stopwords=[u'i', u'me', u'my', u'myself', u'we', u'our', u'ours', u'ourselves', u'you', u'your', u'yours', u'yourself', u'yourselves', u'he', u'him', u'his', u'himself', u'she', u'her', u'hers', u'herself', u'it', u'its', u'itself', u'they', u'them', u'their', u'theirs', u'themselves', u'what', u'which', u'who', u'whom', u'this', u'that', u'these', u'those', u'am', u'is', u'are', u'was', u'were', u'be', u'been', u'being', u'have', u'has', u'had', u'having', u'do', u'does', u'did', u'doing', u'a', u'an', u'the', u'and', u'but', u'if', u'or', u'because', u'as', u'until', u'while', u'of', u'at', u'by', u'for', u'with', u'about', u'against', u'between', u'into', u'through', u'during', u'before', u'after', u'above', u'below', u'to', u'from', u'up', u'down', u'in', u'out', u'on', u'off', u'over', u'under', u'again', u'further', u'then', u'once', u'here', u'there', u'when', u'where', u'why', u'how', u'all', u'any', u'both', u'each', u'few', u'more', u'most', u'other', u'some', u'such', u'no', u'nor', u'not', u'only', u'own', u'same', u'so', u'than', u'too', u'very', u's', u't', u'can', u'will', u'just', u'don', u'should', u'now', u'also', u'above', u'below', u'beyond', u'before', u'after', u'does', u'doesn', u'besides', u'subject', u'would', u'could']
	stopwords = set(stopwords)
	s = [w for w in s2 if not w in stopwords]
	s=( " ".join( s ))
	return s



import os
path= os.getcwd()+'/20_newsgroups'	
import linecache
import collections
import re
from collections import Counter
i=0
v=[]
inputfile = []
classfile= dict()
labelfile= dict()
groupdict= dict()
map1=dict()
from collections import namedtuple
for root, dirs, files in os.walk(path):
	for groupname in dirs:
		for posting in os.listdir(path+ "/"+ groupname):
			filename= path + "/" + groupname + "/" + posting
			f=open(filename,"r")
			lines=f.readlines()
			for line in lines:
				if "subject:" in line.lower():
					break
			if "re:" not in line.lower():
				i+=1
				map1[i]=firstfn(filename)
				if(groupname not in groupdict):
					groupdict[groupname]= len(groupdict)+1
				classfile[i]= groupname
				labelfile[i]= str(groupname) + "_" + posting
			else:
				continue


mydict=dict()
blabel=[]
i=-1
j=0
t=""
f2=open("bag.csv",'w')
f8=open("bag.clabel",'w')
for k,v in map1.items():
	words = re.findall(r'\w+', map1[k])
	for word in words:
		if word not in mydict:
			mydict[word]=len(mydict)+1
			i= i+1
			blabel.append(word)
			
	occurrence=Counter(words)
	for occur,count in occurrence.items():
		s=str(k)+","+str(mydict[occur])+","+ str(count) + "\n"
		f2.write(s)
	
	while (j <= i):
		t= str(j+1) + "," + blabel[j] +"\n"
		j=j+1
		f8.write(t)
		
f2.close()
f8.close()


ngdict=dict()
c3_label=[]

j=0
t=""
f3=open("char3.csv",'w')
f9= open("char3.clabel",'w')
for k,v in map1.items():
	occurrence2=dict()
	for i in range(len(v)-2):
		ngword=v[i:i+3]
		if ngword not in ngdict:
			ngdict[ngword]=len(ngdict)+1
			c3_label.append(ngword)
		c=v.count(ngword)
		occurrence2[ngdict[ngword]]=c
	for occur,count in occurrence2.items():
		s= str(k)+","+ str(occur)+ "," +str(count) + "\n"
		f3.write(s)

stri=""		
for label3 in c3_label:
    stri= label3 + "\n"
    f9.write(stri)
		
f3.close()
f9.close()



ngdict=dict()
i=-1
j=0
t=""
c5_label=[]
f4=open("char5.csv",'w')
f10=open("char5.clabel",'w')
for k,v in map1.items():
	occurrence2=dict()
	for i in range(len(v)-4):
		ngword=v[i:i+5]
		if ngword not in ngdict:
			ngdict[ngword]=len(ngdict)+1
			i= i+1
			c5_label.append(ngword)
			
		c=v.count(ngword)
		occurrence2[ngdict[ngword]]=c
	for occur,count in occurrence2.items():
		s= str(k)+","+ str(occur)+ "," +str(count) + "\n"
		f4.write(s)

stri=""		
for label5 in c5_label:
    stri= label5 + "\n"
    f10.write(stri)	
		
f4.close()
f10.close()



ngdict=dict()

j=0
t=""
c7_label=[]
f11=open("char7.clabel",'w')
f5=open("char7.csv",'w')
for k,v in map1.items():
	occurrence2=dict()
	for i in range(len(v)-6):
		ngword=v[i:i+7]
		if ngword not in ngdict:
			ngdict[ngword]=len(ngdict)+1
			c7_label.append(ngword)
		c=v.count(ngword)
		occurrence2[ngdict[ngword]]=c
	for occur,count in occurrence2.items():
		s= str(k)+","+ str(occur)+ "," +str(count) + "\n"
		f5.write(s)
stri=""		
for label7 in c7_label:
    stri= label7 + "\n"
    f11.write(stri)
		
f11.close()		
f5.close()



f6=open("newsgroups.class",'w')
f7= open("newsgroups.rlabel", 'w')
for k,v in classfile.items():
	s= str(k)+"," + str(v) + "\n"
	f6.write(s)
f6.close()
for k,v in labelfile.items():
	s1= str(k)+"," + str(v) + "\n"
	f7.write(s1)
f7.close()
	
