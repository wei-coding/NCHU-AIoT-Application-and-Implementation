import pymysql.cursors


def main():
    conn = pymysql.connect(host='localhost', user='test123', password='test123', database='aiot', cursorclass=pymysql.cursors.DictCursor)

    with conn:
        with conn.cursor() as cursor:
            sql = "SELECT * FROM `lights` WHERE `temp` > %s"
            cursor.execute(sql, ('20', ))
            result = cursor.fetchall()
            for r in result:
                print(r)


if __name__ == '__main__':
    main()
