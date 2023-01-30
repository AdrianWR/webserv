FROM debian:buster

WORKDIR /

RUN apt update; \
	apt install -y \
	nginx \
	procps \
	vim

COPY ./conf/example_conf /srcs/

RUN mkdir -p /var/www/example; \
  unlink /etc/nginx/sites-enabled/default; \
	mv /srcs/example_conf /etc/nginx/sites-available/ ; \
	ln -s /etc/nginx/sites-available/example_conf etc/nginx/sites-enabled/ ;

EXPOSE 4242

ENTRYPOINT ["nginx", "-g", "daemon off;"]
