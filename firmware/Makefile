# Makefile Final para o projeto MSC25

# --- Configurações do Projeto ---
MCU 		?=	atmega328p
F_CPU		?=	16000000UL
TARGET      =   firmware
OPT			=	s

# --- Diretórios ---
SRCDIR      :=  src
OBJDIR      :=  obj
BINDIR      :=  bin
LIBDIR      :=  lib

# --- Arquivos ---
SRCS	=	$(shell find $(SRCDIR) -type f -name \*.c)
OBJS	=	$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# --- Toolchain ---
CROSS ?= avr-
CC = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
SIZE = $(CROSS)size

# --- Flags de Compilação ---
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -O$(OPT)
CFLAGS += -Wall -Wextra -Wstrict-prototypes
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -std=gnu99
CFLAGS += -I$(SRCDIR)
CFLAGS += -I$(LIBDIR) -I$(LIBDIR)/avr-can-lib

# --- Flags de Linkedição ---
LDFLAGS = -Wl,-Map=$(BINDIR)/$(TARGET).map,--cref
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -L$(LIBDIR)/avr-can-lib/src -lcan

# --- Regras Principais ---
.PHONY: all clean directories flash

# A primeira regra é a padrão. Rodar 'make' executa esta regra 'all'.
all: directories $(BINDIR)/$(TARGET).elf size

# A regra 'clean' é independente e só limpa os arquivos gerados.
clean:
	@echo "Limpando o projeto..."
	@rm -rf $(OBJDIR)/* $(BINDIR)/*

# Regra para gravar o firmware na placa (depende de 'all').
flash: all
	@echo "Gravando na placa..."
	@avrdude -c arduino -P /dev/ttyACM0 -p $(MCU) -D -U flash:w:$(BINDIR)/$(TARGET).elf

# --- Regras Auxiliares ---
directories:
	@mkdir -p $(BINDIR) $(OBJDIR)

size: $(BINDIR)/$(TARGET).elf
	@echo "Tamanho do Firmware:"
	@$(SIZE) -C --mcu=$(MCU) $<

# --- Regras de Compilação e Linkedição ---
$(BINDIR)/$(TARGET).elf: $(OBJS)
	@echo "Linkando o projeto..."
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compilando: $<"
	@$(CC) $(CFLAGS) -c $< -o $@
