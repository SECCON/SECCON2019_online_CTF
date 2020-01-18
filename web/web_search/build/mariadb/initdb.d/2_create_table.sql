USE seccon_sqli;

DROP TABLE IF EXISTS articles;
CREATE TABLE articles (
  id INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
  title VARCHAR(100),
  description VARCHAR(500),
  reference VARCHAR(100) DEFAULT '',
  status INT DEFAULT 0 -- 0: public, 1: private
) ENGINE=InnoDB CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

DROP TABLE IF EXISTS flag;
CREATE TABLE flag (
  piece VARCHAR(500) NOT NULL
) ENGINE=InnoDB CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

ALTER TABLE flag ADD INDEX index_flag_piece(piece);
