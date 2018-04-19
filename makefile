BUILD_DIR 	= ./build
BIN_DIR 	= ./bin
CC 			= gcc
LD 			= ld 
INC 		= -I include/ -I lib/include 
				#-lphread #linux only
CFLAGS		= -Wall $(INC) -c  -W -Wstrict-prototypes -Wmissing-prototypes 
SO_OBJ_CFLAGS = $(CFLAGS) -fPIC
SO_CFLAGS = -shared
#LDFLAGS		= -macosx_version_min 10.12 -e main



SO_OBJS = $(BUILD_DIR)/LzmaDec.o \
			 $(BUILD_DIR)/lzmadecode.o

SO_NAME = lzmadec

SO_TARGET = $(BUILD_DIR)/lib$(SO_NAME).so

EXE_OBJ = $(BUILD_DIR)/main.o

INCLUDE_FILE = lib/include/Precomp.h \
							 lib/include/LzmaDec.h \
							 lib/include/Compiler.h \
							 lib/include/7zTypes.h


PHP_EXT_DIR = `php-config --extension-dir`
PHP_MOD_TARGET = php_module_lzmadec/modules/lzmadec.so
PHP_INI_FILE = /usr/local/php/etc/php.ini


$(BUILD_DIR)/LzmaDec.o : lib/LzmaDec.c $(INCLUDE_FILE)
	$(CC) $(SO_OBJ_CFLAGS) $< -o $@

$(BUILD_DIR)/lzmadecode.o : lzmadecode.c $(INCLUDE_FILE)
	$(CC) $(SO_OBJ_CFLAGS) $< -o $@

$(EXE_OBJ) : main.c $(INCLUDE_FILE)
	$(CC) $(CFLAGS) $< -o $@


$(SO_TARGET) : $(SO_OBJS)
	$(CC) $(SO_CFLAGS) $^ -o $@




#伪目标
.PHONY : mk_dir clean objs all lzmastr bin so_install php_module_install test

php_module_install : objs
	cd php_module_lzmadec && phpize && ./configure && make LDFLAGS=-llzmadec
	cp $(PHP_MOD_TARGET) $(PHP_EXT_DIR)
	echo "[lzmadec]" >> $(PHP_INI_FILE)
	echo "extension=lzmadec.so" >> $(PHP_INI_FILE)



test :
	@echo $(PHP_EXT_DIR) "I dont write test process.just joke"

mk_dir:
	if [[ ! -d $(BUILD_DIR) ]];then mkdir $(BUILD_DIR);fi
	if [[ ! -d $(BIN_DIR) ]];then mkdir $(BIN_DIR);fi

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*
	rm -rf php_module_lzmadec/lzmadec.o
	rm -rf php_module_lzmadec/lzmadec.lo

objs: mk_dir $(SO_OBJS) $(SO_TARGET)

so_install : objs
	cp -rfv $(SO_TARGET) /lib64

$(BIN_DIR)/lzma_dec_str : $(EXE_OBJ) $(SO_TARGET)
	$(CC) $(INC) -o $@ $< -l $(SO_NAME) 


lzma_dec_str : objs so_install $(BIN_DIR)/lzma_dec_str

all : lzma_dec_str php_module_install
