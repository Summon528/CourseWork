SELECT SUM(walkDistance + swimDistance + rideDistance) as totalDistance,  SUM(walkDistance) as walkDistance, SUM(swimDistance) as swimDistance, SUM(rideDistance) as rideDistance 
FROM player_statistic
WHERE Id = ANY (
    SELECT Id
    FROM player_statistic
    WHERE walkDistance <> 0 AND swimDistance <> 0 AND rideDistance <> 0
    GROUP BY Id
)
GROUP BY Id
ORDER BY totalDistance DESC
LIMIT 100;
