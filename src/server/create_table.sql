CREATE TABLE UserInfo
(
    idLog SERIAL PRIMARY KEY NOT NULL,
    login varchar(16) UNIQUE NOT NULL,
    password varchar(256) NOT NULL,
    email varchar(64) NOT NULL,
    admin boolean,
    pathImage varchar(64),
    descr varchar(512)
);

CREATE TABLE Projet
(
    idProjet SERIAL PRIMARY KEY NOT NULL,
    chemin varchar(124) NOT NULL,
    valide boolean,
    nom varchar(64),
    tag varchar(16),
    descr varchar(512),
    pathImage varchar(64),
    idLog INT REFERENCES UserInfo (idLog)
);