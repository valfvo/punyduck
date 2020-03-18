CREATE TABLE UserInfo
(
    idLog INT PRIMARY KEY NOT NULL,
    login varchar(16) UNIQUE NOT NULL,
    password varchar(256) NOT NULL,
    email varchar(64) NOT NULL,
    admin boolean,
    idImage INT REFERENCES Images (idImages),
    idBio INT REFERENCES Bio (idBio)
);

CREATE TABLE Images
(
    idImages INT PRIMARY KEY NOT NULL,
    chemin varchar(64) NOT NULL
);

CREATE TABLE Bio
(
    idBio INT PRIMARY KEY NOT NULL,
    chemin varchar(64) NOT NULL
);

CREATE TABLE Projet
(
    idProjet INT PRIMARY KEY NOT NULL,
    chemin varchar(124) NOT NULL,
    valide boolean,
    nom varchar(64),
    tag varchar(16),
    idLog INT REFERENCES UserInfo (idLog),
    idBio INT REFERENCES Bio (idBio)
);