SCRIPT_DIRECTORY=`dirname $0`
rev=\"`git rev-parse --short HEAD`\"
lastrev=$(head -n 1 $SCRIPT_DIRECTORY/Revision.h | awk -F'PLUGIN_REVISION ' {'print $2'})

echo current revision $rev
echo last build revision $lastrev

if [ "$lastrev" != "$rev" ]
then
   echo "#define PLUGIN_REVISION $rev" > $SCRIPT_DIRECTORY/Revision.h
   echo "#define PLUGIN_REVISION_W L$rev" >> $SCRIPT_DIRECTORY/Revision.h
fi
