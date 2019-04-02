SELECT teamRoadKills, AVG(avgWinPlacePercPerGroup) as avgWinPlacePerc
FROM (
    SELECT matchId, SUM(roadKills) as teamRoadKills, AVG(winPlacePerc) as avgWinPlacePercPerGroup
    FROM player_statistic
    GROUP BY matchId, groupId
) as ps
INNER JOIN `match`
ON match.matchId = ps.matchId
WHERE matchType="squad" OR matchType="squad-fpp"
GROUP BY teamRoadKills
ORDER BY teamRoadKills DESC;

