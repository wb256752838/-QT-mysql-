/*
 Navicat Premium Dump SQL

 Source Server         : a
 Source Server Type    : MySQL
 Source Server Version : 80040 (8.0.40)
 Source Host           : localhost:3306
 Source Schema         : bms

 Target Server Type    : MySQL
 Target Server Version : 80040 (8.0.40)
 File Encoding         : 65001

 Date: 20/12/2024 09:52:03
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for admin
-- ----------------------------
DROP TABLE IF EXISTS `admin`;
CREATE TABLE `admin`  (
  `A_Id` int NOT NULL,
  `A_Zh` int NOT NULL AUTO_INCREMENT,
  `A_Name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `A_Passwd` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `A_Pno` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  PRIMARY KEY (`A_Zh`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 11 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of admin
-- ----------------------------
INSERT INTO `admin` VALUES (1, 1, '1', '1', '28-613-6218');
INSERT INTO `admin` VALUES (2, 7, '刘秀英', '333', '769-7932-5472');
INSERT INTO `admin` VALUES (248, 8, '林睿', 'VMUwuEpy31', '146-1534-6482');
INSERT INTO `admin` VALUES (630, 9, '邱詩涵', 'YptdDb3OW0', '755-340-6622');
INSERT INTO `admin` VALUES (451, 10, '黄安琪', 'EBd2gqMIKS', '185-5049-8651');

-- ----------------------------
-- Table structure for book
-- ----------------------------
DROP TABLE IF EXISTS `book`;
CREATE TABLE `book`  (
  `B_Id` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `B_Name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `B_Cou` int NOT NULL,
  `B_Jg` int NOT NULL,
  `C_Name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  PRIMARY KEY (`B_Id`) USING BTREE,
  INDEX `C_Name`(`C_Name` ASC) USING BTREE,
  CONSTRAINT `book_ibfk_1` FOREIGN KEY (`C_Name`) REFERENCES `cbs` (`C_Name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of book
-- ----------------------------
INSERT INTO `book` VALUES ('1', '红楼梦1', 4, 12, '长沙出版社');
INSERT INTO `book` VALUES ('2', '三国演义2', 5, 24, '怀化出版社');
INSERT INTO `book` VALUES ('3', '西游记3', 2, 23, '香港出版社');
INSERT INTO `book` VALUES ('4', '水浒传4', 5, 34, '英国出版社');
INSERT INTO `book` VALUES ('5', '边城5', 5, 54, '北京出版社');

-- ----------------------------
-- Table structure for borrow
-- ----------------------------
DROP TABLE IF EXISTS `borrow`;
CREATE TABLE `borrow`  (
  `J_Id` int NOT NULL AUTO_INCREMENT,
  `J_Pno` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `J_Xh` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `J_Book` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `J_Cou` int NOT NULL,
  `J_Js` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `J_Hs` datetime NULL DEFAULT NULL,
  `J_Yh` datetime NULL DEFAULT ((now() + interval 30 day)),
  PRIMARY KEY (`J_Id`) USING BTREE,
  UNIQUE INDEX `unique_borrow`(`J_Pno` ASC, `J_Book` ASC) USING BTREE,
  INDEX `borrow_ibfk_1`(`J_Book` ASC) USING BTREE,
  INDEX `borrow_ibfk_2`(`J_Xh` ASC) USING BTREE,
  CONSTRAINT `borrow_ibfk_1` FOREIGN KEY (`J_Book`) REFERENCES `book` (`B_Id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `borrow_ibfk_2` FOREIGN KEY (`J_Xh`) REFERENCES `student` (`S_Id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 25 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of borrow
-- ----------------------------
INSERT INTO `borrow` VALUES (24, '111', '1', '3', 1, '2024-12-20 09:03:59', NULL, '2025-01-19 09:03:59');

-- ----------------------------
-- Table structure for borrow_h
-- ----------------------------
DROP TABLE IF EXISTS `borrow_h`;
CREATE TABLE `borrow_h`  (
  `L_Id` int NOT NULL AUTO_INCREMENT,
  `L_Pno` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `L_Xh` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `L_Book` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `L_Cou` int NOT NULL,
  `L_Js` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `L_Hs` datetime NULL DEFAULT NULL,
  PRIMARY KEY (`L_Id`) USING BTREE,
  INDEX `书籍编号`(`L_Book` ASC) USING BTREE,
  INDEX `学号`(`L_Xh` ASC) USING BTREE,
  CONSTRAINT `borrow_h_ibfk_1` FOREIGN KEY (`L_Book`) REFERENCES `book` (`B_Id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `borrow_h_ibfk_2` FOREIGN KEY (`L_Xh`) REFERENCES `student` (`S_Id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 75 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of borrow_h
-- ----------------------------
INSERT INTO `borrow_h` VALUES (69, '1', '1', '1', 1, '2024-12-19 20:40:23', '2024-12-19 20:40:27');
INSERT INTO `borrow_h` VALUES (71, '1', '1', '3', 1, '2024-12-19 20:40:23', '2024-12-19 20:40:27');
INSERT INTO `borrow_h` VALUES (72, '1', '1', '1', 1, '2024-12-19 20:53:37', '2024-12-19 20:54:37');
INSERT INTO `borrow_h` VALUES (74, '1', '1', '3', 1, '2024-12-19 20:53:37', '2024-12-19 20:54:37');

-- ----------------------------
-- Table structure for cbs
-- ----------------------------
DROP TABLE IF EXISTS `cbs`;
CREATE TABLE `cbs`  (
  `C_Name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `C_Addr` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `C_Yb` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `C_Pno` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  PRIMARY KEY (`C_Name`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of cbs
-- ----------------------------
INSERT INTO `cbs` VALUES ('北京出版社', '北京', '789', '3');
INSERT INTO `cbs` VALUES ('怀化出版社', '怀化', '456', '2');
INSERT INTO `cbs` VALUES ('英国出版社', '英国', '246', '5');
INSERT INTO `cbs` VALUES ('长沙出版社', '长沙', '123', '1');
INSERT INTO `cbs` VALUES ('香港出版社', '香港', '135', '4');

-- ----------------------------
-- Table structure for login
-- ----------------------------
DROP TABLE IF EXISTS `login`;
CREATE TABLE `login`  (
  `S_Id` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `S_Name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `S_Passwd` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `image` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT 'C:UsersqinzhPicturesOIP.bmp',
  PRIMARY KEY (`S_Id`) USING BTREE,
  CONSTRAINT `login_ibfk_1` FOREIGN KEY (`S_Id`) REFERENCES `student` (`S_Id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of login
-- ----------------------------
INSERT INTO `login` VALUES ('1', '1', '1', 'C:UsersqinzhPicturesOIP.bmp');
INSERT INTO `login` VALUES ('2', '常睿', '111', 'C:UsersqinzhPicturesOIP.bmp');
INSERT INTO `login` VALUES ('3', '熊詩涵', '111', 'C:UsersqinzhPicturesOIP.bmp');

-- ----------------------------
-- Table structure for student
-- ----------------------------
DROP TABLE IF EXISTS `student`;
CREATE TABLE `student`  (
  `S_Id` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `S_Name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `S_Class` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `S_Age` int NULL DEFAULT NULL,
  `S_Sex` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `S_Pno` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  PRIMARY KEY (`S_Id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of student
-- ----------------------------
INSERT INTO `student` VALUES ('1', '1', '1', 1, '1', '1');
INSERT INTO `student` VALUES ('2', '常睿', '8', 15, '男', '21-5164-3191');
INSERT INTO `student` VALUES ('3', '熊詩涵', '1', 48, '女', '162-6290-6655');
INSERT INTO `student` VALUES ('4', '邓璐', '8', 43, '女', '179-8681-3459');
INSERT INTO `student` VALUES ('5', '廖璐', '2', 40, '女', '188-7276-5785');

-- ----------------------------
-- View structure for v_book_cbs
-- ----------------------------
DROP VIEW IF EXISTS `v_book_cbs`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `v_book_cbs` AS select `book`.`B_Id` AS `B_Id`,`book`.`B_Name` AS `B_Name`,`book`.`B_Jg` AS `B_Jg`,`book`.`C_Name` AS `C_Name`,`cbs`.`C_Addr` AS `C_Addr`,`cbs`.`C_Yb` AS `C_Yb`,`cbs`.`C_Pno` AS `C_Pno` from (`book` join `cbs` on((`book`.`C_Name` = `cbs`.`C_Name`)));

-- ----------------------------
-- Procedure structure for bor_book
-- ----------------------------
DROP PROCEDURE IF EXISTS `bor_book`;
delimiter ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `bor_book`(IN p_学号 VARCHAR(45),
    IN p_书籍编号 VARCHAR(45),
    IN p_数量 INT,
    IN p_借书凭证 VARCHAR(45))
BEGIN
    DECLARE p_图书库存 INT;
    DECLARE p_借阅凭证存在 INT;

    SELECT `B_COU` INTO p_图书库存 FROM `book` WHERE `B_ID` = p_书籍编号;
    IF p_图书库存 IS NULL THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '本图书馆未收藏该书';
    ELSE
        IF p_图书库存 < p_数量 THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '图书库存不足';
        ELSE
            SELECT COUNT(*) INTO p_借阅凭证存在 FROM `borrow` WHERE `J_Pno` = p_借书凭证 AND `J_Book` = p_书籍编号;
            IF p_借阅凭证存在 > 0 THEN
                SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '该借书凭证已存在，请更换借书凭证';
            ELSE
                IF EXISTS (SELECT 1 FROM `borrow` WHERE `J_Book` = p_书籍编号 AND `J_Xh` = p_学号 AND `J_Hs` IS NULL) THEN
                    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '您已借了该图书';
                ELSE
                    INSERT INTO `borrow` (`J_Xh`, `J_Book`, `J_Cou`, `J_Pno`, `J_Js`) VALUES (p_学号, p_书籍编号, p_数量, p_借书凭证, CURRENT_TIMESTAMP);
                    UPDATE `book` SET `B_Cou` = `B_Cou` - p_数量 WHERE `B_Id` = p_书籍编号;
                END IF;
            END IF;
        END IF;
    END IF;
END
;;
delimiter ;

-- ----------------------------
-- Procedure structure for giv_book
-- ----------------------------
DROP PROCEDURE IF EXISTS `giv_book`;
delimiter ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `giv_book`(IN p_学号 VARCHAR(45), IN p_书籍编号 VARCHAR(45), IN p_数量 INT, IN p_借书凭证 VARCHAR(45))
BEGIN
    DECLARE p_借阅记录 INT;
    DECLARE p_还书日期 DATETIME;
		DECLARE p_图书库存 INT;
		DECLARE p_借阅数量 INT;
		

    SELECT `J_Cou` INTO p_图书库存 FROM `borrow` WHERE `J_Xh` = p_学号 AND `J_Book` = p_书籍编号 AND `J_Hs` IS NULL 		AND `J_Pno` = p_借书凭证;
    -- 设置p_还书日期为当前时间
    SET p_还书日期 = CURRENT_TIMESTAMP;

    -- 检查是否有有效的借阅记录
    SELECT COUNT(*) INTO p_借阅记录 FROM `borrow` WHERE `J_Xh` = p_学号 AND `J_Book` = p_书籍编号 AND `J_Hs` IS NULL AND `J_Pno` = p_借书凭证 AND p_数量 <= p_图书库存;
    IF p_借阅记录 = 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '无有效的借阅记录';
    ELSE
		
				UPDATE `borrow` SET `J_Cou` = `J_Cou` - p_数量 WHERE `J_Pno` = p_借书凭证 AND `J_Book` = p_书籍编号;
        -- 更新还书日期为当前时间
        UPDATE `borrow` SET `J_Hs` = p_还书日期 WHERE `J_Pno` = p_借书凭证 AND `J_Book` = p_书籍编号 AND `J_Cou`= 0;
				
        -- 更新book的数量
        UPDATE `book` SET `B_Cou` = `B_Cou` + p_数量 WHERE `B_Id` = p_书籍编号;
				CALL `删除已还书记录`(p_数量);
    END IF;
END
;;
delimiter ;

-- ----------------------------
-- Procedure structure for 删除已还书记录
-- ----------------------------
DROP PROCEDURE IF EXISTS `删除已还书记录`;
delimiter ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `删除已还书记录`(IN p_数量 INT)
BEGIN
    -- 插入还书日期不为NULL的记录到历史已还书记录表中
    INSERT INTO `borrow_h`(`L_Pno`, `L_Xh`, `L_Book`, `L_Cou`, `L_Js`, `L_Hs`)
    SELECT `J_Pno`, `J_Xh`, `J_Book`, p_数量, `J_Js`, `J_Hs`
    FROM `borrow`
    WHERE `J_Hs` IS NOT NULL;

    -- 删除borrow中还书日期不为NULL的记录
    DELETE FROM `borrow` WHERE `J_Hs` IS NOT NULL;
END
;;
delimiter ;

SET FOREIGN_KEY_CHECKS = 1;
