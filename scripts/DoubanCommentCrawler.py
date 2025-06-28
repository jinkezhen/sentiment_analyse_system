# import json
# import os
# import requests
# from urllib.request import urlretrieve
# import time

# sleeptime = 1  # 休眠时间

# # 浏览器访问伪装
# headers = {
#     'cookie': '__jdv=122270672%7Cdirect%7C-%7Cnone%7C-%7C1748748065782; shshshfpa=c56de574-2f20-defb-7def-e7795cfe1e75-1748748066; shshshfpx=c56de574-2f20-defb-7def-e7795cfe1e75-1748748066; jcap_dvzw_fp=JVzyEPv_HEVtOxYrE96fr6sTLMNW5AemK5gsl6sczz8Ru9QtsbduLw7Vz_KsJatIutOvm3q6njfkwG3CD6wy2Q==; jsavif=1; areaId=3; pinId=tNpHVzOiMe70iVPuRnQYEw; pin=jd_seeazdwBIYgT; unick=%E4%B8%80%E7%BB%99%E6%88%91%E9%87%8C_giaogaio; _tp=CO0M7oi2%2Fr00p3I90qZSHg%3D%3D; _pst=jd_seeazdwBIYgT; mail_times=2%2C1; PCSYCityID=CN_120000_120100_0; umc_count=1; ipLoc-djd=3-51046-55624-0; 3AB9D23F7A4B3C9B=EXLVYRPLG726QBTAODR5PZV2FAIPNPIMWZ3TQGUDNP7SZ3EYDUFIRDJPXJZENTGM7UGTSXWJPEHL4XP7UF7LJC44A4; TrackID=14uYcQdTgjiC8f_cmBKXUQ2MJqoxt4DPfCnk5Un5o5D4VjJ526-yysCZ5-Xq-J-shu8MT2kHGAdhKxrM6-sf1Omey4Yrx9N8ykgGrOokw9-1NOKAoIoMUil2WwMqse4HC; thor=E7545DA6E863098CAD48AB83A6885925F2ABF54109609B140B990C16A0F60B93F522FE0C04E993A040D782BA957A7A4FEA93986EFF2DE29C58D271E2DE2E374E06A9723331CB82371CB0F448A43F839752EA1AED01AC16A27C92D88C19B1E5C9D4CF472F582529C7D205DDAB7DCE598D117C2E96508026D85959D48EABB53BEA68A6E9E6CE1125183844404D64EE39545905DE4B14A0832B0E29650A713F6901; light_key=AASBKE7rOxgWQziEhC_QY6yaWNnC5IGW_n2uGFeIaj0YXqu79uWzcp-qw1OrvXxNXNXoy7zz; cn=1; flash=3_9JxYOR0_jxCF-C7wOFgdaKvF8dtw-cNb1yQGWbZSV4Hxr2ZK6TqyI6ME7pxO3qsLkTvStHIne7Yun4fIi1mkoLqOU_gBTJDCbGCTHc_SIO9Ju8BM1TIq5pllG5Ge5CkbNCIGAhy46-6AmTG5Y7NHsU2KGZFZXgmqX1Yj3yOdtCByr7yxjZE7; 3AB9D23F7A4B3CSS=jdd03EXLVYRPLG726QBTAODR5PZV2FAIPNPIMWZ3TQGUDNP7SZ3EYDUFIRDJPXJZENTGM7UGTSXWJPEHL4XP7UF7LJC44A4AAAAMXFHAT7QIAAAAADIFGYUMQXNRRHAX; _gia_d=1; token=1b6b6266e3e56c7fe76670253cc5afcb,3,971529; __jda=181111935.17487480657811122492395.1748748065.1748748065.1748748065.1; __jdc=181111935; __jdb=181111935.22.17487480657811122492395|1.1748748065; __jdu=17487480657811122492395; sdtoken=AAbEsBpEIOVjqTAKCQtvQu17HO6V3EKdb85BI2CZVVmVjPDoUzrvqDqY0xiuxqc74olwq736lc-s8JxeZ5WapGC2J3S3l5isYAMFH8mgWcoo_GTAl67EDLciO5I9pEb82BbZWw; shshshfpb=BApXSbRTJKvJAm0tk3WcwT88mjg7nOK-vBgo1h0lo9xJ1Mp78Jo62',
#     'referer': 'https://item.jd.com/',
#     'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.77 Safari/537.36 Edg/91.0.864.37',
# }

# jd_prod_pic_save_path = 'jd_prod_pic'  # 图片保存目录

# page_range = 50   # 翻50页取500条记录

# def getJDProdComment(prod_skuId):
#     big_pag_path = '//img30.360buyimg.com/shaidan/s616x405_jfs/'  # 评价大图地址
#     tiny_pag_path = '//img30.360buyimg.com/n0/s128x96_jfs/'       # 评价缩略图地址
#     for i in range(page_range):
#         comments_url = 'https://api.m.jd.com/?appid=item-v3&functionId=pc_club_productPageComments&client=pc&clientVersion=1.0.0&t=1705853247039&loginType=3&uuid=181111935.16953916127351737253418.1695391613.1705847111.1705852812.35&productId='+str(prod_skuId)+'&score=0&sortType=5&page='+str(i)+'&pageSize=10&isShadowSku=0&fold=1&bbtf=&shield='
#         data = requests.get(comments_url, headers=headers)
#         if data:
#             content_data=data.content.decode('utf-8')
#             json_data=json.loads(content_data)
#             print(json_data)
#             if i == 0:
#                 productCommentSummary=json_data.get('productCommentSummary')
#                 print(prod_skuId, str(productCommentSummary))
#                 hotCommentTagStatistics=json_data.get('hotCommentTagStatistics')
#                 print(prod_skuId,str(hotCommentTagStatistics))
#             comments=json_data.get('comments')
#             if comments and len(comments)>0:
#                 for comment in comments:
#                     prod_comment_guid=comment.get('guid')
#                     prod_comment_content=str(comment.get('content'))
#                     prod_comment=str(comment)
#                     images=comment.get('images')
#                     savepath=os.path.join(jd_prod_pic_save_path,str(prod_skuId),'comments',prod_comment_guid)
#                     if images and len(images)>0:
#                         for image in images:
#                             str_image_url=image.get('imgUrl').replace(tiny_pag_path, big_pag_path)
#                             image_url='https:'+str_image_url
#                             filename=os.path.basename(image_url)
#                             downloadfile(image_url, savepath, filename) #下载图片
#                             time.sleep(sleeptime)
#                     print(prod_comment_guid,prod_skuId,prod_comment_content,prod_comment)
#             time.sleep(sleeptime)


# def downloadfile(downloadurl,savepath,savefilename):
#     savefile = os.path.join(savepath, savefilename)
#     try:
#         if not os.path.exists(savepath):
#             os.makedirs(savepath)
#             # 判断文件是否存在，如果不存在则下载
#         if not os.path.isfile(savefile):
#             print('Downloading data from %s' % downloadurl)
#             urlretrieve(downloadurl, filename=savefile)
#             print('\nDownload finished!')
#         else:
#             print('File already exsits!')
#         # 获取文件大小
#         filesize = os.path.getsize(savefile)
#         # 文件大小默认以Bytes计， 转换为Mb
#         print('File size = %.2f Mb' % (filesize / 1024 / 1024))
#     except Exception as e:
#         print('downloadfile Error:', e)

# if __name__ == '__main__':
#     getJDProdComment('100087971268')


# from selenium import webdriver
# from selenium.webdriver.common.by import By
# from selenium.webdriver.chrome.options import Options
# from selenium.webdriver.chrome.service import Service
# from bs4 import BeautifulSoup
# import time

# # 配置无头浏览器（可选）
# chrome_options = Options()
# chrome_options.add_argument('--headless')  # 无头模式，可视化可注释掉
# chrome_options.add_argument('--disable-gpu')
# chrome_options.add_argument('--no-sandbox')
# chrome_options.add_argument('--window-size=1920,1080')
# chrome_options.add_argument('--disable-blink-features=AutomationControlled')

# # 初始化驱动
# driver = webdriver.Chrome(options=chrome_options)

# # 要爬取的页面
# url = "https://movie.douban.com/subject/1291546/comments"

# # 打开网页
# driver.get(url)
# time.sleep(2)  # 等待页面加载

# all_comments = []

# # 设置要爬取的页数
# max_pages = 10

# for page in range(max_pages):
#     print(f"正在爬取第 {page + 1} 页...")

#     # 解析页面
#     soup = BeautifulSoup(driver.page_source, 'html.parser')
#     comment_spans = soup.find_all('span', class_='short')

#     for span in comment_spans:
#         all_comments.append(span.get_text(strip=True))

#     # 尝试点击“后页”按钮翻页
#     try:
#         next_btn = driver.find_element(By.LINK_TEXT, "后页>")
#         next_btn.click()
#         time.sleep(2)  # 等待下一页加载
#     except Exception as e:
#         print("没有更多页面或点击失败：", e)
#         break

# # 关闭浏览器
# driver.quit()

# # 输出评论结果
# print("\n=== 共获取评论 %d 条 ===\n" % len(all_comments))
# for idx, comment in enumerate(all_comments, 1):
#     print(f"{idx:03d}. {comment}")


# from selenium import webdriver
# from selenium.webdriver.common.by import By
# from selenium.webdriver.chrome.options import Options
# from selenium.webdriver.chrome.service import Service
# from bs4 import BeautifulSoup
# import time

# # 无头浏览器配置（可选）
# chrome_options = Options()
# chrome_options.add_argument('--headless')  # 可注释以调试
# chrome_options.add_argument('--disable-gpu')
# chrome_options.add_argument('--no-sandbox')
# chrome_options.add_argument('--window-size=1920,1080')
# chrome_options.add_argument('--disable-blink-features=AutomationControlled')

# # 启动 WebDriver
# driver = webdriver.Chrome(options=chrome_options)

# # 初始页面
# url = "https://movie.douban.com/subject/1291546/comments"

# driver.get(url)
# time.sleep(2)

# all_comments = []

# # 设置最大页数
# max_pages = 10

# for page in range(max_pages):
#     print(f"正在爬取第 {page + 1} 页...")

#     # 使用 BeautifulSoup 解析评论内容
#     soup = BeautifulSoup(driver.page_source, 'html.parser')
#     comment_spans = soup.find_all('span', class_='short')
#     for span in comment_spans:
#         all_comments.append(span.get_text(strip=True))

#     # 翻页：查找“下一页”按钮
#     try:
#         next_button = driver.find_element(By.CSS_SELECTOR, 'a.next')
#         # 如果按钮不可点击或为 disabled 状态
#         if 'href' not in next_button.get_attribute("outerHTML"):
#             print("已到最后一页")
#             break
#         next_button.click()
#         time.sleep(2)
#     except Exception as e:
#         print("没有更多页面或点击失败：", e)
#         break

# driver.quit()

# # 输出所有评论
# print(f"\n=== 共获取评论 {len(all_comments)} 条 ===\n")
# for idx, comment in enumerate(all_comments, 1):
#     print(f"{idx:03d}. {comment}")




# from selenium import webdriver
# from selenium.webdriver.common.by import By
# from selenium.webdriver.chrome.options import Options
# from bs4 import BeautifulSoup
# import time

# # 无头浏览器配置（可选）
# chrome_options = Options()
# chrome_options.add_argument('--headless')  # 可注释以调试
# chrome_options.add_argument('--disable-gpu')
# chrome_options.add_argument('--no-sandbox')
# chrome_options.add_argument('--window-size=1920,1080')
# chrome_options.add_argument('--disable-blink-features=AutomationControlled')

# # 启动 WebDriver
# driver = webdriver.Chrome(options=chrome_options)

# # 初始页面
# url = "https://movie.douban.com/subject/36478801/comments"
# driver.get(url)
# time.sleep(2)

# all_comments = []
# page_count = 1

# while True:
#     print(f"正在爬取第 {page_count} 页...")

#     # 使用 BeautifulSoup 解析评论内容
#     soup = BeautifulSoup(driver.page_source, 'html.parser')
#     comment_spans = soup.find_all('span', class_='short')
#     for span in comment_spans:
#         all_comments.append(span.get_text(strip=True))

#     # 查找“下一页”按钮
#     try:
#         next_button = driver.find_element(By.CSS_SELECTOR, 'a.next')
#         next_button.click()
#         page_count += 1
#         time.sleep(2)
#     except Exception as e:
#         print("以加在所有数据")
#         break

# driver.quit()

# # 输出所有评论
# print(f"\n=== 共获取评论 {len(all_comments)} 条 ===\n")
# for idx, comment in enumerate(all_comments, 1):
#     print(f"{idx:03d}. {comment}")


# import argparse
# from selenium import webdriver
# from selenium.webdriver.common.by import By
# from selenium.webdriver.chrome.options import Options
# from bs4 import BeautifulSoup
# import time

# def scrape_douban_comments(movie_id):
#     # 无头浏览器配置（可选）
#     chrome_options = Options()
#     chrome_options.add_argument('--headless')  # 可注释以调试
#     chrome_options.add_argument('--disable-gpu')
#     chrome_options.add_argument('--no-sandbox')
#     chrome_options.add_argument('--window-size=1920,1080')
#     chrome_options.add_argument('--disable-blink-features=AutomationControlled')

#     # 启动 WebDriver
#     driver = webdriver.Chrome(options=chrome_options)

#     # 初始页面
#     url = f"https://movie.douban.com/subject/{movie_id}/comments"
#     driver.get(url)
#     time.sleep(2)

#     all_comments = []
#     page_count = 1

#     while True:
#         print(f"正在爬取第 {page_count} 页...")

#         # 使用 BeautifulSoup 解析评论内容
#         soup = BeautifulSoup(driver.page_source, 'html.parser')
#         comment_spans = soup.find_all('span', class_='short')
#         for span in comment_spans:
#             all_comments.append(span.get_text(strip=True))

#         # 查找"下一页"按钮
#         try:
#             next_button = driver.find_element(By.CSS_SELECTOR, 'a.next')
#             next_button.click()
#             page_count += 1
#             time.sleep(2)
#         except Exception as e:
#             print("已加载所有数据")
#             break

#     driver.quit()

#     # 自动生成输出文件名
#     output_file = f"{movie_id}.txt"
    
#     # 保存评论到文件
#     with open(output_file, 'w', encoding='utf-8') as f:
#         for comment in all_comments:
#             f.write(comment + '\n')

#     print(f"\n=== 共获取评论 {len(all_comments)} 条 ===\n")
#     print(f"评论已保存到 {output_file}")

# if __name__ == "__main__":
#     # 设置命令行参数
#     parser = argparse.ArgumentParser(description='豆瓣电影评论爬取工具')
#     parser.add_argument('movie_id', help='豆瓣电影ID，如36478801')
    
#     args = parser.parse_args()
    
#     # 调用爬取函数
#     scrape_douban_comments(args.movie_id)



# import argparse
# from selenium import webdriver
# from selenium.webdriver.common.by import By
# from selenium.webdriver.chrome.options import Options
# from bs4 import BeautifulSoup
# import time
# import pymysql
# from datetime import datetime

# def get_db_connection():
#     try:
#         connection = pymysql.connect(
#             host='127.0.0.1',
#             user='root',  
#             password='123456',
#             database='sentiment_analysis',
#             charset='utf8mb4',
#             cursorclass=pymysql.cursors.DictCursor
#         )
#         return connection
#     except Exception as e:
#         print(f"数据库连接失败: {e}")
#         return None

# def init_database():
#     connection = get_db_connection()
#     if connection is None:
#         return False
    
#     try:
#         with connection.cursor() as cursor:
#             cursor.execute("""
#                 CREATE TABLE IF NOT EXISTS movies (
#                     movie_id VARCHAR(20) PRIMARY KEY,
#                     created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
#                     rating_ratio DECIMAL(5,2) DEFAULT 0.00
#                 )
#             """)
            
#             cursor.execute("""
#                 CREATE TABLE IF NOT EXISTS comments (
#                     id INT AUTO_INCREMENT PRIMARY KEY,
#                     movie_id VARCHAR(20),
#                     content TEXT,
#                     created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
#                     FOREIGN KEY (movie_id) REFERENCES movies(movie_id)
#                 )
#             """)
        
#         connection.commit()
#         return True
#     except Exception as e:
#         print(f"数据库初始化失败: {e}")
#         return False
#     finally:
#         connection.close()

# def save_to_database(movie_id, comments):
#     connection = get_db_connection()
#     if connection is None:
#         return False
    
#     try:
#         with connection.cursor() as cursor:
#             # 插入或更新电影记录
#             cursor.execute("""
#                 INSERT INTO movies (movie_id) 
#                 VALUES (%s)
#                 ON DUPLICATE KEY UPDATE created_at = CURRENT_TIMESTAMP
#             """, (movie_id,))
            
#             # 批量插入评论数据
#             cursor.executemany(
#                 "INSERT INTO comments (movie_id, content) VALUES (%s, %s)",
#                 [(movie_id, comment) for comment in comments]
#             )
        
#         connection.commit()
#         print(f"成功保存 {len(comments)} 条评论到数据库")
#         return True
#     except Exception as e:
#         print(f"数据库保存失败: {e}")
#         return False
#     finally:
#         connection.close()

# def scrape_douban_comments(movie_id):
#     # 无头浏览器配置
#     chrome_options = Options()
#     chrome_options.add_argument('--headless')
#     chrome_options.add_argument('--disable-gpu')
#     chrome_options.add_argument('--no-sandbox')
#     chrome_options.add_argument('--window-size=1920,1080')
#     chrome_options.add_argument('--disable-blink-features=AutomationControlled')

#     driver = webdriver.Chrome(options=chrome_options)

#     # 初始页面
#     url = f"https://movie.douban.com/subject/{movie_id}/comments"
#     driver.get(url)
#     time.sleep(2)

#     all_comments = []
#     page_count = 1

#     while True:
#         print(f"正在爬取第 {page_count} 页...")

#         soup = BeautifulSoup(driver.page_source, 'html.parser')
#         comment_spans = soup.find_all('span', class_='short')
#         for span in comment_spans:
#             all_comments.append(span.get_text(strip=True))

#         try:
#             next_button = driver.find_element(By.CSS_SELECTOR, 'a.next')
#             next_button.click()
#             page_count += 1
#             time.sleep(2)
#         except Exception as e:
#             print("已加载所有数据")
#             break
    
#     driver.quit()

#     if save_to_database(movie_id, all_comments):
#         print(f"\n=== 共获取评论 {len(all_comments)} 条 ===\n")
#         print(f"评论已保存到MySQL数据库")
#     else:
#         print("评论保存失败")

# if __name__ == "__main__":
#     parser = argparse.ArgumentParser(description='豆瓣电影评论爬取工具')
#     parser.add_argument('movie_id', help='豆瓣电影ID，如36478801')
#     args = parser.parse_args()

#     if not init_database():
#         print("数据库初始化失败，请检查MySQL服务是否正常运行")
#         exit(1)
        
#     scrape_douban_comments(args.movie_id)



import argparse
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from bs4 import BeautifulSoup
import time
import pymysql
from datetime import datetime
import os
import re

def get_db_connection():
    try:
        connection = pymysql.connect(
            host='127.0.0.1',
            user='root',  
            password='123456',
            database='sentiment_analysis',
            charset='utf8mb4',
            cursorclass=pymysql.cursors.DictCursor
        )
        return connection
    except Exception as e:
        print(f"数据库连接失败: {e}")
        return None

def init_database():
    connection = get_db_connection()
    if connection is None:
        return False
    
    try:
        with connection.cursor() as cursor:
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS movies (
                    movie_id VARCHAR(20) PRIMARY KEY,
                    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                    rating_ratio DECIMAL(5,2) DEFAULT 0.00
                )
            """)
            
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS comments (
                    id INT AUTO_INCREMENT PRIMARY KEY,
                    movie_id VARCHAR(20),
                    content TEXT,
                    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                    FOREIGN KEY (movie_id) REFERENCES movies(movie_id)
                )
            """)
        
        connection.commit()
        return True
    except Exception as e:
        print(f"数据库初始化失败: {e}")
        return False
    finally:
        connection.close()

def save_to_database(movie_id, comments):
    connection = get_db_connection()
    if connection is None:
        return False
    
    try:
        with connection.cursor() as cursor:
            # 插入或更新电影记录
            cursor.execute("""
                INSERT INTO movies (movie_id) 
                VALUES (%s)
                ON DUPLICATE KEY UPDATE created_at = CURRENT_TIMESTAMP
            """, (movie_id,))
            
            # 批量插入评论数据
            cursor.executemany(
                "INSERT INTO comments (movie_id, content) VALUES (%s, %s)",
                [(movie_id, comment) for comment in comments]
            )
        
        connection.commit()
        print(f"成功保存 {len(comments)} 条评论到数据库")
        return True
    except Exception as e:
        print(f"数据库保存失败: {e}")
        return False
    finally:
        connection.close()

def save_to_txt(movie_id, comments, output_dir):
    """将评论保存到指定目录下的movie_id.txt文件中"""
    # 确保输出目录存在
    os.makedirs(output_dir, exist_ok=True)
    
    # 构建文件路径
    file_path = os.path.join(output_dir, f"{movie_id}.txt")
    
    try:
        with open(file_path, 'w', encoding='utf-8') as f:
            for comment in comments:
                # 关键修改：移除所有换行符和回车符，确保每条评论独占一行
                cleaned_comment = re.sub(r'[\r\n]+', ' ', comment).strip()
                f.write(cleaned_comment + '\n')
        print(f"成功保存 {len(comments)} 条评论到文件: {file_path}")
        return True
    except Exception as e:
        print(f"保存到文件失败: {e}")
        return False

def scrape_douban_comments(movie_id, output_dir):
    # 无头浏览器配置
    chrome_options = Options()
    chrome_options.add_argument('--headless')
    chrome_options.add_argument('--disable-gpu')
    chrome_options.add_argument('--no-sandbox')
    chrome_options.add_argument('--window-size=1920,1080')
    chrome_options.add_argument('--disable-blink-features=AutomationControlled')

    driver = webdriver.Chrome(options=chrome_options)

    # 初始页面
    url = f"https://movie.douban.com/subject/{movie_id}/comments"
    driver.get(url)
    time.sleep(2)

    all_comments = []
    page_count = 1

    while True:
        print(f"正在爬取第 {page_count} 页...")

        soup = BeautifulSoup(driver.page_source, 'html.parser')
        comment_spans = soup.find_all('span', class_='short')
        for span in comment_spans:
            all_comments.append(span.get_text(strip=True))

        try:
            next_button = driver.find_element(By.CSS_SELECTOR, 'a.next')
            next_button.click()
            page_count += 1
            time.sleep(2)
        except Exception as e:
            print("已加载所有数据")
            break
    
    driver.quit()

    # 保存到数据库
    db_success = save_to_database(movie_id, all_comments)
    
    # 保存到文本文件
    file_success = save_to_txt(movie_id, all_comments, output_dir)
    
    print(f"\n=== 共获取评论 {len(all_comments)} 条 ===")
    if db_success:
        print(f"评论已保存到MySQL数据库")
    if file_success:
        print(f"评论已保存到文本文件: {os.path.join(output_dir, f'{movie_id}.txt')}")
    if not db_success and not file_success:
        print("评论保存失败")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='豆瓣电影评论爬取工具')
    parser.add_argument('movie_id', help='豆瓣电影ID，如36478801')
    parser.add_argument('output_dir', nargs='?', default='/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/crawler_data', 
                        help='输出目录，默认为 /home/jkz/project/new_sentiment_analyse_system/project_with_mysql/crawler_data')
    
    args = parser.parse_args()

    # 确保输出目录存在
    os.makedirs(args.output_dir, exist_ok=True)
    
    if not init_database():
        print("数据库初始化失败，请检查MySQL服务是否正常运行")
        # 即使数据库失败也继续爬取，但只保存到文件
        print("将继续爬取并保存到文本文件")
        
    scrape_douban_comments(args.movie_id, args.output_dir)