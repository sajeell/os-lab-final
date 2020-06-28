# GROUP MEMBERS ARE:
# 1. Sajeel Ahmad Hassan 01-131182-032
# 2. Shaheer Shahzad 01-131182-025
NAME = OSLABFINAL

SRC = OSLABFINAL.cpp

SRCPATH=./
OBJ=$(addprefix $(SRCPATH), $(SRC:.cpp=.o))

RM=rm -f
INCPATH=includes
CPPFLAGS+= -I $(INCPATH)


all: $(OBJ)
	g++ $(OBJ) -o $(NAME) -lpthread

clean:
	-$(RM) *~
	-$(RM) *#*
	-$(RM) *swp
	-$(RM) *.core
	-$(RM) *.stackdump
	-$(RM) $(SRCPATH)*.o
	-$(RM) $(SRCPATH)*.obj
	-$(RM) $(SRCPATH)*~
	-$(RM) $(SRCPATH)*#*
	-$(RM) $(SRCPATH)*swp
	-$(RM) $(SRCPATH)*.core
	-$(RM) $(SRCPATH)*.stackdump
	-$(RM) $(NAME)

fclean: clean
	-$(RM) $(NAME)

re: fclean all

