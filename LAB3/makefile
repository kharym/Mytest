DIR_OUTPUT = ./build
DIR_SRC = ./src
NOMBRE_EJECUTABLE = Laboratorio3

# ESTA ES LA TARGET QUE SE EJECUTA POR DEFECTO SI SE ESCRIBE "MAKE" EN LA CONSOLA.
all: release

# COMPILACION PRINCIPLA DEL EJECUTABLE
release:
	@echo 'Compilando target: $@'
	mkdir -p $(DIR_OUTPUT)
	gcc $(DIR_SRC)/main.c -l pthread $(DIR_SRC)/funciones.c -l pthread $(DIR_SRC)/procedimientos.c -std=gnu99 -o $(DIR_OUTPUT)/$(NOMBRE_EJECUTABLE)
	@echo ' '
	@echo $@ compilado en el directorio $(DIR_OUTPUT)
	@echo "Ingrese al directorio '"$(DIR_OUTPUT)"' haciendo: cd "$(DIR_OUTPUT)
	@echo "Ejecute el programa agregando -c Cantidad de imagenes -h Cantidad de hebras -u Umbral para binarizar -n Umbral para clasificar -b a: ./"$(NOMBRE_EJECUTABLE)
	@echo ' '

# COMPILACION CON OPCIONES DE DEBUGEO Y SIN OPTIMIZACIONES
debug:
	@echo 'Compilando target: $@'
	mkdir -p $(DIR_OUTPUT)
	gcc $(DIR_SRC)/main.c -l pthread $(DIR_SRC)/funciones.c -l pthread $(DIR_SRC)/procedimientos.c -o $(DIR_OUTPUT)/$(NOMBRE_EJECUTABLE) -g3 -Wall -pedantic -DDEBUG
	@echo ' '
	@echo $@ compilado en el directorio $(DIR_OUTPUT)
	@echo "Ingrese al directorio '"$(DIR_OUTPUT)"' haciendo: cd "$(DIR_OUTPUT)
	@echo "Ejecute el programa agregando -c Cantidad de imagenes -h Cantidad de hebras -u Umbral para binarizar -n Umbral para clasificar -b a: ./"$(NOMBRE_EJECUTABLE)
	@echo ' '

# OTROS
clean:
# BORRO EL CONTENIDO DEL DIRECTORIO DE COMPILACION
	rm -rf $(DIR_OUTPUT)/*
