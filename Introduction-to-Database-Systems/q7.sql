SELECT SUM(numberOfPlayersPerMatch) as numberOfPlayers, Max(maxHealsPerMatch) as maxHeals 
FROM (
    SELECT matchId, COUNT(*) as numberOfPlayersPerMatch, MAX(heals) as maxHealsPerMatch
    FROM player_statistic
    WHERE damageDealt = 0 AND winPlacePerc = 1.0
    GROUP BY matchId
) as ps
INNER JOIN `match`
ON ps.matchId = match.matchId
WHERE matchDuration > (
    SELECT AVG(matchDuration)
    FROM `match`
);
