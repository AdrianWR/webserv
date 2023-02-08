# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lniehues <lniehues@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/21 18:58:50 by lniehues          #+#    #+#              #
#    Updated: 2023/01/29 20:14:27 by lniehues         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

IMAGE		:=	nginx
CONTAINER	:=	nginx

ROOT_PATH = $(shell pwd)

build:
	docker build -t $(IMAGE) .

build_no_cache:
	docker build --no-cache=true -t $(IMAGE) .

run_d:
	docker run -d --rm -p 4242:4242 -p 4243:4243 --name $(CONTAINER) -v "$(ROOT_PATH)/srcs:/var/www/example" -v "$(ROOT_PATH)/conf:/etc/nginx/sites-enabled" $(IMAGE)

run:
	docker run --rm -p 4242:4242 -p 4243:4243 --name $(CONTAINER) -v "$(ROOT_PATH)/srcs:/var/www/example" -v "$(ROOT_PATH)/conf:/etc/nginx/sites-enabled" $(IMAGE)

exec_bash:
	docker exec -it $(CONTAINER) /bin/bash

stop:
	docker stop $(CONTAINER)

.PHONY:	build run exec stop
