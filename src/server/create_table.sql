CREATE TABLE UserInfo
(
    idLog SERIAL PRIMARY KEY NOT NULL,
    login varchar(16) UNIQUE NOT NULL,
    password varchar(256) NOT NULL,
    email varchar(64) NOT NULL,
    admin boolean,
    uPathImage varchar(256),
    uDescr varchar(512)
);

CREATE TABLE Projet
(
    idProjet SERIAL PRIMARY KEY NOT NULL,
    chemin varchar(256) NOT NULL,
    valide boolean,
    nom varchar(64),
    tag varchar(16),
    -- note int(10),
    -- nbNote int,
    pDescr varchar(512),
    pPathImage varchar(256),
    pIdLog INT REFERENCES UserInfo (idLog)
);