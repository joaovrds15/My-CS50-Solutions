SELECT name FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE movies.id IN
                    (SELECT movies.id FROM movies
                    JOIN stars S ON movies.id = S.movie_id
                    JOIN people P ON S.person_id = P.id
                    WHERE P.name = "Kevin Bacon" and P.birth = 1958)
                    AND people.name != "Kevin Bacon";