import re
import requests
import urllib.request
import signal, os
import smtplib
import time
from email.mime.text import MIMEText


# url = "http://software.khu.ac.kr/board5/bbs/board.php?bo_table=05_01"
# received = requests.get(url)
#
# text = received.content
# text = text.decode('utf-8')
#
# notice_number_list = re.findall(r'<td class="td_num2">(.+?)</td>', text, re.DOTALL)

most_recent_notice_number = 90
check_interval = 60 * 60


def crawler(url):
    global most_recent_notice_number
    received = requests.get(url)

    text = received.content
    text = text.decode('utf-8')

    isNotify = False
    notice_number_list = re.findall(r'<td class="td_num2">(.+?)</td>', text, re.DOTALL)
    notice_date_list = re.findall(r'<td class="td_datetime">(.+?)</td>', text, re.DOTALL)
    notice_list_html = re.findall(r'<div class="bo_tit">(.+?)</div>', text, re.DOTALL)

    temp_most_recent_notice_number = int(notice_number_list[0].strip())
    if temp_most_recent_notice_number > most_recent_notice_number:
        isNotify = True
        notice_count = temp_most_recent_notice_number - most_recent_notice_number
        notice_list = []
        for i in range(notice_count):
            notice_title = re.findall(r'>(.+?)</a>',notice_list_html[i], re.DOTALL)
            notice_url = re.findall(r'<a href="(.+?)">', notice_list_html[i], re.DOTALL)
            notice_date = notice_date_list[i]
            notice_json = {
                'title': notice_title[0].strip(),
                'url': notice_url[0],
                'date': notice_date,
            }

            notice_list.append(notice_json)
        most_recent_notice_number = temp_most_recent_notice_number
        return { 'isNotify' : isNotify, 'notice_list' : notice_list }

    else:
        return { 'isNotify' : isNotify }


def main():  # client 단

    def sigAlrmHandler(signum=None, frame=None):
        results = crawler(url)
        print(results)
        if results['isNotify']:
            from_address = 'khunotifier@gmail.com'
            to_address = user_email
            content = []
            for index, notice in enumerate(results['notice_list']):
                content.append("%d. (%s) <a href='%s'>%s</a>" % (index + 1, "금일 "+notice['date'] if ":" in notice['date'] else notice['date'], notice['url'], notice['title']))
            content = "<br>".join(content)
            msg = MIMEText(content, 'html')
            msg['Subject'] = 'KHNotifier 공지사항 알림이 메일'
            msg['From'] = from_address
            msg['To'] = to_address

            google_server = smtplib.SMTP_SSL('smtp.gmail.com', 465)
            google_server.login('doublejtoh@khu.ac.kr', 'wnsgus3535')
            google_server.sendmail(msg['From'], [msg['To']], msg.as_string())
        signal.alarm(check_interval)


    print("유저 이메일을 입력해주세요.")
    user_email = input() # 나중에 db logic 추가
    signal.signal(signal.SIGALRM, sigAlrmHandler)
    signal.alarm(check_interval) # 1시간 뒤 alarm handler 수행
    print(most_recent_notice_number)
    url = "http://software.khu.ac.kr/board5/bbs/board.php?bo_table=05_01"
    sigAlrmHandler()
    while(1):
        time.sleep(check_interval)

if __name__ == '__main__':
    main()
