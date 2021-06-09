# dflograph
data flow equation visualization tool

This is dflo, a data-flow equation visualization and experimentation tool.  
Given a directed graph (nodes and edges) and a set of simple assignments   
within each node (scalar left hand side, simple expression of scalars  
and integer constants on right hand side), and a set of simple boolean  
data-flow equations, it displays the flow graph, allows a user to   
  (1) select an expression, whereupon it displays the nodes which contain  
      a computation of that expression and which contain definitions that  
      kill that expression; and  
  (2) select a data-flow equation, whereupon it highlights the nodes  
      and/or edges where the data-flow solution for the selected expression  
      is True.  


FLOW GRAPH INPUT:  
The flow graph is given as a list of edges, and a list of simple assignments  
in each node.  Assignments can have empty left or right hand sides.  
A sample flow graph is:  
	(entry,b1)  
	(b1,b2)  
	(b2,b3)  
	(b2,b4)  
	(b3,b5)  
	(b4,b5)  
	(b5,b2)  
	(b5,b6)  
	(b6,exit)  
	b1[a=0,i=0]  
	b2[i=i+1,b=]  
	b3[b=,=i]  
	b4[a=a+1,=b+a,=i]  
	b5[=b+a]  
	b6[=b+a,=a]  

The tool can be used to simply display flow graphs; different kinds of  
graph edges can be displayed, such as:  
	b2..>b3		(dotted edge)  
	b2<--b3		(arrow pointing backward)  
	b2--b3		(no arrow)  
	b2<-->b3)	(double arrowed edge)  
	b2..b3		(dotted edge, no arrow)  

The original C++ surce is in directory original  
The syntax of the tool with html pages is in directory syntax  
The graphical interface is in one single file dgplot.cc for motif on X11 on Linux  
This dgplot.cc file can be updated for postscript output or other gui toolkit  

The example input graph files are in the *.in files  
The dflo.mk file is the makefile  



