# Smart-Traffic-Light
Smart traffic light using C++

Dự án đèn giao thông thông minh tại ngã tư sử dụng C++ gồm mã nguồn DU_AN_DEN_GIAO_THONG_FINAL.cpp và web mô phỏng thuật toán là
DEN_GIAO_THONG_THONG_MINH.html chỉ cần tải web mô phỏng về và mở trong trình duyệt để xem dự án.
Web mô phỏng sử dụng mã nguồn chính là mã nguồn DU_AN_DEN_GIAO_THONG_FINAL.cpp

Về cơ bản dự án sẽ hoạt động như sau: 
1.Cập nhật số lượng xe tại từng làn, có 4 làn là đông-tây-nam-bắc tương ứng với 4 đèn giao thông tại 4 làn.
    Trong phần này cũng sẽ cập nhật xem có xe ưu tiên trong các làn hay không.
    Sau khi đã có đầy đủ thông tin thì sẽ tính toán và đưa ra kết quả mật độ tại các làn, làn nào được ưu tiên nhất, số điểm ưu tiên 
    tại các làn tương ứng và số giây đếm ngược đèn tại các làn xe.
2.1 Trong trường hợp có xe ưu tiên:
    + Nếu xe ưu tiên đang ở làn có đèn xanh: thì giữ đèn xanh tại làn đó cho đến khi xe chạy ra khỏi làn thì đèn hoạt động trở lại
      bình thường.
    + Nếu xe ưu tiên đang ở trong làn có đèn đỏ: thì đèn xanh ở làn đối diện sẽ giảm xuống còn 5s tương ứng với đèn đỏ ở làn có xe ưu
      tiên sẽ giảm xuống còn 8s(5s đỏ + 3s vàng) sau đó chuyển xanh để xe ưu tiên đi ra khỏi làn và đèn hoạt động trở lại bình thường.
2.2Trong trường hợp không có xe ưu tiên: thì sẽ tính mật độ xe tại từng pha đối diện nhau(đông-tây hoặc nam bắc).
    + Nếu pha nào có mật độ lớn hơn pha còn lại 60% thì đèn xanh tịa làn có mạt độ lớn hơn sẽ chạy trong 50s, còn đèn xanh tại làn có 
      mật độ ít hơn sẽ chạy trong 15s.
    + Nếu mật độ tại 2 pha chênh lệch nhau <60% thì đèn xanh đỏ tại các làn tương ứng sẽ là 30s/30s.
3. Các giá trị số lượng xe và xe ưu tiên tại các làn sẽ được cập nhật sau chu kì mỗi 5s.
   Sau đó sẽ tính toán và đưa ra kết quả như phần 1 và chạy đèn xanh đỏ sao cho phù hợp.

   
Với lượng kiến thức hạn hẹp đây là dự án dựa trên phỏng đoán và mô phỏng chứ không dựa trên bất kì 1 ngã tư thực tế nào nếu 
có bất kì sai sót nào mong cô và các bạn thông cảm.
