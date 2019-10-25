filepath=$1
egrep -o '_[A-Z][a-zA-Z_]+_[a-zA-Z_]+' $filepath | sort | uniq | grep -v UNUSED
