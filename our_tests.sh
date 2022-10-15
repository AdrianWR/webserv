#!/bin/bash
echo "Choose test: "
echo "1  GET: metodo permitido"
echo "2  GET: tem redirection"
echo "3  GET: arquivo existe"
echo "4  GET: arquivo nao existe"
echo "5  GET: diretorio com index"
echo "6  GET: diretorio sem index com autoindex"
echo "7  GET: diretorio sem index e sem autoindex"
echo "8 GET: CGI (executa arquivo.py)"
echo "9  POST: Metodo permitido / Post arquivo"
echo "99 POST: Metodo permitido / Chunked"
echo "10 POST: Maior que body size"
echo "11 POST: Metodo nao permitido"
echo "12 POST: Post dir"
#CGI
echo "14 DELETE: Metodo Permitido"
echo "15 DELETE: Metodo nao Permitido"
echo "16 DELETE: Arquivo nao existe"
echo "17 DELETE: Arquivo existe mas nao tem permissao"


read n

# ===========================================================
# GET
# ===========================================================
if [ $n == 1 ]; then
# "1  GET: metodo permitido"
	curl -v www.site1.com:8081/getok/site1.html
fi
if [ $n == 2 ]; then
# "2  GET: tem redirection"
	curl -v www.site1.com:8081/redirection/
fi
if [ $n == 3 ]; then
# "3  GET: arquivo existe"
	curl -v www.site1.com:8081/getok/site1.html
fi
if [ $n == 4 ]; then
# "4  GET: arquivo nao existe"
	curl -v www.site1.com:8081/getok/arquivonaoexiste.html
fi
if [ $n == 5 ]; then
# "5  GET: diretorio com index"
	curl -v www.site1.com:8081/diretorio/index.html
fi
if [ $n == 6 ]; then
# "6  GET: diretorio sem index com autoindex"
	curl -v www.site1.com:8081/outrodir/
fi
if [ $n == 7 ]; then
#"7  GET: diretorio sem index e sem autoindex"
	curl -v www.site1.com:8081/getok/
fi
if [ $n == 8 ]; then
#8 GET: CGI (executa arquivo.py)"
	curl -v www.site1.com:8081/cgi/pag_cgi.py
fi

# ===========================================================
# POST
# ===========================================================
if [ $n == 9 ]; then
# "9  POST: Metodo permitido / Post arquivo"
	curl -v -H "body:12345" -X POST www.site2.com:8082/uploadok/aaa.txt
fi
if [ $n == 99 ]; then
# "99 POST: Metodo permitido / Chunked"
	python3 ./clients/client.py
fi
if [ $n == 10 ]; then
# "10 POST: Maior que body size"
	curl -v -H "content-length: 10000" -X POST www.site2.com:8082/uploadok/aaa.txt
fi
if [ $n == 11 ]; then
# "11 POST: Metodo nao permitido"
	curl -v -H "body:12345" -X POST www.site2.com:8082/noupload/aaa.txt
fi
if [ $n == 12 ]; then
# "12 POST: Post dir"
	curl -v -X POST www.site2.com:8082/diretorio/
fi

# ===========================================================
# DELETE
# ===========================================================
if [ $n == 14 ]; then
# "14 DELETE: Metodo Permitido"
	curl -v -X DELETE www.site3.com:8083/deleteok/arq1
fi
if [ $n == 15 ]; then
# "15 DELETE: Metodo nao Permitido"
	curl -v -X DELETE www.site3.com:8083/deletenok/arq1
fi
if [ $n == 16 ]; then
# "16 DELETE: Arquivo nao existe"
	curl -v -X DELETE www.site3.com:8083/deleteok/arq4
fi
if [ $n == 17 ]; then
# "17 DELETE: Arquivo existe mas nao tem permissao"
	curl -v -X DELETE www.site3.com:8083/sempermissao/arq1
fi



# SEGUNDO TESTE DA INTRA:

#POST / HTTP/1.1
#Host: localhost:5555
#User-Agent: Go-http-client/1.1
#Transfer-Encoding: chunked
#Content-Type: test/file
#Accept-Encoding: gzip
if [ $n == 20 ]; then
	curl -v -H "Host: localhost:5555" -H "User-Agent: Go-http-client/1.1" -H "Transfer-Encoding: chunked" -H "Content-Type: test/file" -H "Accept-Encoding: gzip" -X POST http://localhost:8081
fi

# 1o teste 2GET: 200

# 2o teste POST chunked: 405
# 3o teste HEAD: 405

# 4o teste GET/directory: 200
# 5o teste GET/directory/youpi.bad_extension: 200
# 6o teste GET/directory/youpi.bla: 200

# 7o teste GET/directory.oulalala: 404

# 8o teste GET/direcory/nop: 200
# 9o teste GET/directory/nop/:200
# 10o teste GET/directory/nop/other.pouic: 200

# 11o teste GET/directory/nop/other.pouac: 404
# 12o teste GET/directory/Yeah: 404

# 13o teste GET/directory/Yeah/not_happy.bad_extension: 200
# 14o teste PUT_test/file_should_exist_after: 



