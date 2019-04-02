SELECT numGroups, match.matchId, kills, COUNT(*) as playerCount
FROM player_statistic as ps
INNER JOIN `match`
ON match.matchId = ps.matchId
WHERE 
    numGroups = (
        SELECT MAX(numGroups) FROM `match`
    ) AND (
         SELECT COUNT( DISTINCT ps2.kills)
         FROM player_statistic as ps2
        WHERE ps2.kills > ps.kills AND ps.matchId = ps2.matchId
    ) < 3
GROUP BY matchId, kills
ORDER BY matchId, kills DESC;
