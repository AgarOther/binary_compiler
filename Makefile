# Compilation
CC					=	c++
CFLAGS				=	-Wall -Wextra -Werror -std=c++17 -MP -MMD

# Names
NAME				=	binary_compiler

# Sources & Includes
SRCS				= 	srcs/main.cpp
OBJ_FOLDER			=	objs
INCLUDES			=	-I srcs/

# Objects
OBJS				=	$(patsubst %.cpp, $(OBJ_FOLDER)/%.o, $(SRCS))
DEPS				=	$(patsubst %.cpp, $(OBJ_FOLDER)/%.d, $(SRCS)) \

# Custom Makefile Flags
MAKEFLAGS			+=	--no-print-directory --silent

# Custom Colors
PURPLE				=	\033[1;35m
LIGHT_GREEN			=	\033[1;32m
RESET				=	\033[0m

# Custom messages
EXE_DONE			=	@echo "🎉$(PURPLE) $(NAME) compiled! 🎉$(RESET)\n"
ALL_CLEAN			=	@echo "🧹$(LIGHT_GREEN) Project's objects cleaned! 🧹$(RESET)\n"
ALL_FCLEAN			=	@echo "🧹$(LIGHT_GREEN) Project's objects & Executables cleaned! 🧹$(RESET)\n"

# Rules

all :$(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	$(EXE_DONE)

$(OBJ_FOLDER)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean :
	@rm -rf $(OBJ_FOLDER)
	$(ALL_CLEAN)

fclean :
	@rm -f $(NAME)
	@rm -rf $(OBJ_FOLDER)
	$(ALL_FCLEAN)

re : fclean all

.PHONY: all clean fclean re

-include $(DEPS)
