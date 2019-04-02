SELECT Id, AVG(kills) as avgKills
FROM player_statistic
INNER JOIN `match`
ON player_statistic.matchId = match.matchId
WHERE numGroups <= 10
GROUP BY Id
ORDER BY avgKills DESC
LIMIT 20;

