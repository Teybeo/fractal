NAME = libft.a

SRCS += ft_memcpy.c
SRCS += ft_memzero.c
SRCS += ft_memmove.c
SRCS += ft_strlen.c
SRCS += ft_puts.c
SRCS += ft_strequ.c

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror
FLAGS += -O2 -march=native -flto

all: $(NAME)

# $^ is the dependencies of the rule
# $@ is the name of the rule
%.o : %.c
	gcc $(FLAGS) -c $^

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all