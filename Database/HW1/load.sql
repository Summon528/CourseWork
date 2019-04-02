CREATE DATABASE HW1;

USE HW1;

CREATE TABLE `match` (
    matchId VARCHAR(20),
    matchDuration INT,
    matchType VARCHAR(20),
    maxPlace INT,
    numGroups INT);

LOAD DATA LOCAL INFILE 'match.csv'
    INTO TABLE `match`
    FIELDS TERMINATED BY ','
    IGNORE 1 LINES;

CREATE TABLE player_statistic (
    Id VARCHAR(20),
    groupId VARCHAR(20),
    matchId VARCHAR(20),
    assists INT,
    boosts INT,
    damageDealt DOUBLE,
    DBNOs INT,
    headshotKills INT,
    heals INT, 
    killPlace INT,
    killPoints INT,
    kills INT,
    killStreaks INT,
    longestKill DOUBLE,
    rankPoints INT,
    revives INT,
    rideDistance DOUBLE,
    roadKills INT,
    swimDistance DOUBLE,
    teamKills INT,
    vehicleDestroys INT,
    walkDistance DOUBLE,
    weaponsAcquired INT,
    winPoints INT,
    winPlacePerc DOUBLE);

LOAD DATA LOCAL INFILE 'player_statistic.csv'
    INTO TABLE player_statistic
    FIELDS TERMINATED BY ','
    IGNORE 1 LINES;

ALTER TABLE `match` MODIFY matchId VARCHAR(20) PRIMARY KEY;
