SRC= confFile.cpp Response.cpp mime.cpp Request.cpp server.cpp utils.cpp worker.cpp SocketS.cpp cppv1.cpp
NAME= f
FLAGS=  -g3 -fsanitize=address 

all: $(SRC)
	c++ $(SRC) -o $(NAME) $(FLAGS) 
fclean:
	rm -rf $(NAME)
re: fclean all
