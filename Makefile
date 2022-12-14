SRC= confFile.cpp Response.cpp mime.cpp Request.cpp server.cpp utils.cpp \
	Crud.cpp worker.cpp SocketS.cpp Delete.cpp cppv1.cpp partOfResponse.cpp
NAME= f
FLAGS=  -g3 -fsanitize=address

all: $(SRC)
	c++ $(SRC) -o $(NAME) $(FLAGS) 
fclean:
	rm -rf $(NAME)
re: fclean all

f: re
	./f confFolder/Conf
