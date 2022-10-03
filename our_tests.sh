#!/bin/bash
echo "Choose test: "
echo "1  GET: metodo permitido"
echo "2  GET: tem redirection"
echo "3  GET: arquivo existe"
echo "4  GET: arquivo nao existe"
echo "5  GET: diretorio com index"
echo "6  GET: diretorio sem index com autoindex"
echo "7  GET: diretorio sem index e sem autoindex"
#CGI
echo "9  POST: Metodo permitido / Post arquivo"
echo "10 POST: Maior que body size"
echo "11 POST: Metodo nao permitido"
echo "12 POST: Post dir"
#CGI
echo "14 DELETE: Metodo Permitido"
echo "15 DELETE: Arquivo existe"
echo "16 DELETE: Arquivo nao existe"
echo "17 DELETE: Arquivo existe mas nao tem permissao"


read n

# ===========================================================
# GET
# ===========================================================
if [ $n == 1 ]; then
	curl -v www.site1.com:8081/getok/site1.html
fi
if [ $n == 2 ]; then
	curl -v www.site1.com:8081/redirection/
fi
if [ $n == 3 ]; then
	curl -v www.site1.com:8081/getok/site1.html
fi
if [ $n == 4 ]; then
	curl -v www.site1.com:8081/getok/arquivonaoexiste.html
fi
if [ $n == 5 ]; then
	curl -v www.site1.com:8081/diretorio/index.html
fi
if [ $n == 6 ]; then
	curl -v www.site1.com:8081/diretorioa/
fi
if [ $n == 7 ]; then
	curl -v www.site1.com:8081/getok/
fi

# ===========================================================
# POST
# ===========================================================
if [ $n == 9 ]; then
	curl -v -X POST www.site2.com:8082/uploadok/aaa.txt
fi
if [ $n == 10 ]; then
	echo 10
fi
if [ $n == 11 ]; then
	curl -v -X POST www.site2.com:8082/noupload/aaa.txt
fi
if [ $n == 12 ]; then
	curl -v -X POST www.site2.com:8082/diretorio/
fi

# ===========================================================
# DELETE
# ===========================================================
if [ $n == 14 ]; then
	curl -v -X DELETE www.site3.com:8083/deleteok/arq1
fi
if [ $n == 15 ]; then
	curl -v -X DELETE www.site3.com:8083/deletenok/arq1
fi
if [ $n == 16 ]; then
	curl -v -X DELETE www.site3.com:8083/deleteok/arq4
fi
if [ $n == 17 ]; then
	curl -v -X DELETE www.site3.com:8083/sempermissao/arq1
fi
