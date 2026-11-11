# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/05 18:38:14 by rexposit          #+#    #+#              #
#    Updated: 2026/11/10 17:24:23 by rexposit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += --no-print-directory

NAME = ircserv

SRC_DIR = ./src
OBJ_DIR = ./obj

CXX = c++

CXXFLAGS =	-Wall -Wextra -Werror \
			-fdiagnostics-color=always \
			-std=c++98 \
			-Iinc

# > ~ Sources

SRCS =	main.cpp \
		Server.cpp \
		Client.cpp

# > ~ .cpp to .o conversion

SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# > ~ Colors!

RED = \033[0;31m
BRED = \033[1;31m
GREEN = \033[0;32m
BGREEN = \033[1;32m
YLW = \033[0;33m
BYLW = \033[1;33m
BLUE = \033[0;36m
GRAY = \033[0;30m
RES = \033[0m\033[K

WNAME = $(BYLW)$(NAME)$(YLW)
OKNAME = $(BLUE)$(NAME)$(GREEN)

# > ~ Compilation function
# Usage: $(call compile_file,source,object)
define compile_file
	@mkdir -p $(dir $(2))
	@printf "\r⏳ $(YLW)Compiling $(BYLW)$(notdir $(1))$(GRAY)...$(RES)"
	@ERR=$$( ($(CXX) $(CXXFLAGS) -c $(1) -o $(2)) 2>&1 ); \
	if [ $$? -ne 0 ]; then \
		printf "\r\n❌ $(RED)Failed to compile $(BRED)$(notdir $(1))$(GRAY):$(RES)\n"; \
		echo "$$ERR"; \
		exit 1; \
	else \
		printf "\r✅ $(GREEN)Compiled $(BGREEN)$(notdir $(1))$(RES)\n"; \
	fi
endef

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@printf "\r✅ $(OKNAME) successfully compiled!$(RES)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(call compile_file,$<,$@)

# > ~ Cleaning

clean:
	@printf "\r⏳ $(YLW)Removing $(WNAME) objs$(GRAY)...$(RES)"
	@rm -rf $(OBJ_DIR)
	@printf "\r✅ $(GREEN)Removed $(OKNAME) objs$(GRAY).$(RES)\n"

fclean: clean
	@printf "\r⏳ $(YLW)Removing $(WNAME) executable$(GRAY)...$(RES)"
	@rm -f $(NAME)
	@printf "\r✅ $(GREEN)Removed $(OKNAME) executable$(GRAY).$(RES)\n"

re: fclean all

.PHONY: all clean fclean re
