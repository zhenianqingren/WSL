filename=$1
point=`expr index "$filename" .`
end=`expr $point - 1`
binname=${filename:0:$end}


