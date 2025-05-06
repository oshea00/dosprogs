@ECHO OFF
masm /l %1
link %1 @autolink
exe2bin %1 %1.com

