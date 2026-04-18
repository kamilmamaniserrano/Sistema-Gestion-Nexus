CREATE DATABASE IF NOT EXISTS nexus_academy;
USE nexus_academy;

CREATE TABLE alumnos (
    id INT PRIMARY KEY,
    nombre VARCHAR(50),
    apellido VARCHAR(50),
    celular VARCHAR(15),
    nota_final DECIMAL(4,2)
);

-- Insertamos 3 alumnos de prueba para empezar
INSERT INTO alumnos (id, nombre, apellido, celular, nota_final) VALUES
(1, 'Ana', 'Gomez', '1122334455', 8.5),
(2, 'Carlos', 'Perez', '5544332211', 3.2),
(3, 'Maria', 'Lopez', '9988776655', 9.8);