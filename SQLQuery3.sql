
--############################### NHÂN VIÊN ###########################################################
-- Tạo bảng Nhân Viên (Thêm căn cước nhân viên)
CREATE TABLE NhanVien (
    MaNhanVien NVARCHAR(10) PRIMARY KEY,
    HoTen NVARCHAR(50) NOT NULL,
	SoDienThoai NVARCHAR(15) NOT NULL UNIQUE, -- Thêm số điện thoại
    CanCuocCongDan NVARCHAR(20) UNIQUE  NOT NULL, -- Thêm căn cước công dân
    VaiTro NVARCHAR(10) CHECK (VaiTro IN ('1', '2')) NOT NULL -- LeTan là 1, ThuNgan là 2 ///            ""DONE""
);

--############################### GIẢM GIÁ ###########################################################
-- Tạo bảng Giảm Giá (Thêm số lượng giảm giá) --- chi can luu so tien // cái này ko cần phần trăm giảm giá
CREATE TABLE GiamGia (
    MaGiamGia NVARCHAR(10) PRIMARY KEY,
    MoTa NVARCHAR(100) NOT NULL,
    SoTienGiam DECIMAL(18, 2),
    NgayBatDau DATE NOT NULL,
    NgayKetThuc DATE NOT NULL,
    SoLuong INT NOT NULL DEFAULT 0,
    CONSTRAINT CK_GiamGia_Ngay CHECK (NgayKetThuc > NgayBatDau) -- Kiểm tra ngày
);


--############################### KHÁCH HÀNG ###########################################################
-- Tạo bảng Khách Hàng (Thêm căn cước công dân, không Unique Email)
CREATE TABLE KhachHang (
    MaKhachHang NVARCHAR(10) PRIMARY KEY, --tất cả mã dùng identity (MaKhachHang INT IDENTITY(1,1) PRIMARY KEY, //DONE
    HoTen NVARCHAR(50) NOT NULL, 
    CanCuocCongDan NVARCHAR(20) NOT NULL UNIQUE , 
    SoDienThoai NVARCHAR(15) NOT NULL UNIQUE,
    Email NVARCHAR(50),
    DiaChi NVARCHAR(100),
    MaGiamGia NVARCHAR(10) NULL,
    FOREIGN KEY (MaGiamGia) REFERENCES GiamGia(MaGiamGia)
);


-- Bảng trung gian để khách hàng có nhiều voucher cùng lúc
CREATE TABLE KhachHang_GiamGia (
    MaKhachHang NVARCHAR(10),
    MaGiamGia NVARCHAR(10),
    PRIMARY KEY (MaKhachHang, MaGiamGia),
    FOREIGN KEY (MaKhachHang) REFERENCES KhachHang(MaKhachHang), --       "DONE"
    FOREIGN KEY (MaGiamGia) REFERENCES GiamGia(MaGiamGia)
);

--############################### PHÒNG ###########################################################
CREATE TABLE Phong (
    MaPhong NVARCHAR(10) PRIMARY KEY,
    LoaiPhong NVARCHAR(50) NOT NULL, 
    GiaPhong DECIMAL(18,2) NOT NULL, 
    TinhTrang NVARCHAR(1) CHECK (TinhTrang IN ('1', '2', '3', '4')) NOT NULL, -- 1: Trống, 2: Đang sửa, 3: Đang vệ sinh, 4: Đã có người đặt
    SoLuongPhong INT NOT NULL DEFAULT 1,
    Tang INT NOT NULL, -- Thêm trường tầng
    HinhAnhPhong NVARCHAR(100) -- Thêm cột để lưu URL hình ảnh
);

--######################## Hình Ảnh Phòng #######################################################-- KHÔI BÉO LÀM CÁI NÀY "INSERT INTO"
CREATE TABLE HinhAnhChiTietPhong (
    IDANH INT IDENTITY(1,1) PRIMARY KEY, 
    MaPhong NVARCHAR(10) NOT NULL, 
    URLHinhAnh NVARCHAR(100) NOT NULL, 
    FOREIGN KEY (MaPhong) REFERENCES Phong(MaPhong) ON DELETE CASCADE
);



--############################### ĐẶT PHÒNG ###########################################################
-- Tạo bảng Đặt Phòng
CREATE TABLE DatPhong (
    MaDatPhong NVARCHAR(10) PRIMARY KEY,
    MaKhachHang NVARCHAR(10) NOT NULL,
    MaNhanVien NVARCHAR(10) NOT NULL,
    NgayDat DATETIME NOT NULL DEFAULT GETDATE(),
    NgayNhanPhong DATETIME NOT NULL,
    NgayTraPhong DATETIME NOT NULL,
    TrangThai NVARCHAR(1) CHECK (TrangThai IN ('1', '2')) NOT NULL, -- 1: Xác nhận, 2: Hủy  /// DONE
    FOREIGN KEY (MaKhachHang) REFERENCES KhachHang(MaKhachHang),
    FOREIGN KEY (MaNhanVien) REFERENCES NhanVien(MaNhanVien)
);

-- Bảng trung gian giữa Đặt Phòng và Phòng
CREATE TABLE DatPhong_Phong (
    MaDatPhong NVARCHAR(10),
    MaPhong NVARCHAR(10),
    PRIMARY KEY (MaDatPhong, MaPhong),
    FOREIGN KEY (MaDatPhong) REFERENCES DatPhong(MaDatPhong),			--DONE
    FOREIGN KEY (MaPhong) REFERENCES Phong(MaPhong)
);

--############################### DỊCH VỤ ###########################################################
CREATE TABLE DichVu (
    MaDichVu NVARCHAR(10) PRIMARY KEY,
    TenDichVu NVARCHAR(100) NOT NULL,
    DonGia DECIMAL(18,2) NOT NULL,
    HinhAnhDichVu NVARCHAR(100) -- Thêm cột để lưu URL hình ảnh
);


-- Tạo bảng Chi Tiết Dịch Vụ
CREATE TABLE ChiTietDichVu (
    MaChiTietDichVu NVARCHAR(10) PRIMARY KEY,
    MaDatPhong NVARCHAR(10) NOT NULL,
    MaDichVu NVARCHAR(10) NOT NULL,
    SoLuong INT NOT NULL CHECK (SoLuong > 0),
    ThanhTien DECIMAL(18,2) NOT NULL,
    FOREIGN KEY (MaDatPhong) REFERENCES DatPhong(MaDatPhong),			--DONE
    FOREIGN KEY (MaDichVu) REFERENCES DichVu(MaDichVu)
);

--############################### THANH TOÁN ###########################################################
-- Tạo bảng Hóa Đơn Thanh Toán -- can thêm 1 trường số tiền giảm để ta có // móc 1 thằng mã khách hàng vô #MaKhachHang 
CREATE TABLE HoaDonThanhToan (
    MaHoaDon NVARCHAR(10) PRIMARY KEY,
    MaDatPhong NVARCHAR(10) NOT NULL,
    MaNhanVien NVARCHAR(10) NOT NULL,
    NgayLapHoaDon DATETIME NOT NULL DEFAULT GETDATE(),
    TongTien DECIMAL(18,2) CHECK (TongTien >= 0) NOT NULL,
    TrangThaiThanhToan NVARCHAR(1) CHECK (TrangThaiThanhToan IN ('0', '1', '2')) NOT NULL, -- 0: Chưa thanh toán, 1: Đã thanh toán, 2: Đã hủy
    HinhThucThanhToan INT CHECK (HinhThucThanhToan IN (1, 2)) NOT NULL, -- 1: Tiền mặt, 2: Chuyển khoản  
    MaGiamGia NVARCHAR(10) NULL, -- Mã giảm giá được áp dụng (nếu có)
    GiamGiaApDung DECIMAL(18,2) DEFAULT 0, -- Số tiền giảm từ voucher
    VAT DECIMAL(5,2) CHECK (VAT BETWEEN 0 AND 100) DEFAULT 10, -- Thuế VAT %
    FOREIGN KEY (MaDatPhong) REFERENCES DatPhong(MaDatPhong),
    FOREIGN KEY (MaNhanVien) REFERENCES NhanVien(MaNhanVien),
    FOREIGN KEY (MaGiamGia) REFERENCES GiamGia(MaGiamGia)            --//DONE
	
);


--############################### ĐĂNG NHẬP ###########################################################

-- Tạo bảng Đăng Nhập dùng chung cho Nhân Viên và Khách Hàng
CREATE TABLE DangNhap (
    TenDangNhap NVARCHAR(20) PRIMARY KEY,
    MatKhau NVARCHAR(50) NOT NULL,
    LoaiNguoiDung NVARCHAR(10) CHECK (LoaiNguoiDung IN ('1', '2')) NOT NULL, -- 2: Nhân viên, 1: Khách hàng
    MaNhanVien NVARCHAR(10) NULL,
    MaKhachHang NVARCHAR(10) NULL,
    FOREIGN KEY (MaNhanVien) REFERENCES NhanVien(MaNhanVien),
    FOREIGN KEY (MaKhachHang) REFERENCES KhachHang(MaKhachHang),
    CHECK (
        (LoaiNguoiDung = '1' AND MaNhanVien IS NOT NULL AND MaKhachHang IS NULL) OR
        (LoaiNguoiDung = '2' AND MaKhachHang IS NOT NULL AND MaNhanVien IS NULL)			--DONE
    )
);


CREATE TABLE TaiKhoan (
    TenDangNhap NVARCHAR(20) PRIMARY KEY,  
    MatKhau NVARCHAR(255) NOT NULL,  
    LoaiTaiKhoan NVARCHAR(10) CHECK (LoaiTaiKhoan IN ('1', '2')) NOT NULL, --1 là nhân viên 2 là khách hàng 
    MaKhachHang NVARCHAR(10) NULL,  
    MaNhanVien NVARCHAR(10) NULL,  
    TrangThai NVARCHAR(1) CHECK (TrangThai IN ('0', '1')) DEFAULT '1' NOT NULL,  
    FOREIGN KEY (MaKhachHang) REFERENCES KhachHang(MaKhachHang) ON DELETE CASCADE,  
    FOREIGN KEY (MaNhanVien) REFERENCES NhanVien(MaNhanVien) ON DELETE CASCADE,  
    CONSTRAINT UQ_TaiKhoan UNIQUE (MaKhachHang, MaNhanVien) -- Đảm bảo chỉ có một tài khoản			//DONE
);






--############################################################################################
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--#############################################################################################


select * from Phong
DROP TABLE IF EXISTS Phong;
SELECT 
    fk.name AS ForeignKeyName,
    tp.name AS ParentTable,
    cp.name AS ColumnName,
    tr.name AS ReferencedTable
FROM 
    sys.foreign_keys AS fk
JOIN 
    sys.foreign_key_columns AS fkc ON fk.object_id = fkc.constraint_object_id
JOIN 
    sys.tables AS tp ON fk.parent_object_id = tp.object_id
JOIN 
    sys.columns AS cp ON tp.object_id = cp.object_id AND fkc.parent_column_id = cp.column_id
JOIN 
    sys.tables AS tr ON fk.referenced_object_id = tr.object_id
WHERE 
    tr.name = 'Phong';

	ALTER TABLE DatPhong DROP CONSTRAINT FK__DatPhong___MaPho__5070F446;



	
--############################################################################################
--////////////////////////////////////////////////////////////////////////////////////////////
--//////////////////////////							///////////////////////////////////////
--/////////////////////////      DATA TRUY VẤN			///////////////////////////////////////
--//////////////////////////							///////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--#############################################################################################

--###################################  NHÂN VIÊN   ##########################################################

-- Dữ liệu cho bảng Nhân Viên
INSERT INTO NhanVien (MaNhanVien, HoTen, SoDienThoai, CanCuocCongDan, VaiTro) VALUES
(N'N001', N'Nguyễn Văn A', N'0987654321', N'123456789012', N'1'),
(N'N002', N'Trần Thị B', N'0976543210', N'234567890123', N'2'),
(N'N003', N'Lê Hoàng C', N'0965432109', N'345678901234', N'1'),
(N'N004', N'Phạm Minh D', N'0954321098', N'456789012345', N'2');

-- Dữ liệu cho bảng Giảm Giá
INSERT INTO GiamGia (MaGiamGia, MoTa, SoTienGiam, NgayBatDau, NgayKetThuc, SoLuong) VALUES
(N'GG001', N'Giảm 100,000 VND cho khách hàng mới', 100000, '2025-01-01', '2025-06-30', 50),
(N'GG002', N'Voucher giảm 200,000 VND', 200000, '2025-02-01', '2025-07-31', 30),
(N'GG003', N'Giảm ngay 300,000 VND khi đặt phòng trước', 300000, '2025-03-01', '2025-08-31', 20),
(N'GG004', N'Giảm ngay 500,000 VND cho hóa đơn trên 5 triệu', 500000, '2025-04-01', '2025-09-30', 15);


-- Dữ liệu cho bảng Khách Hàng
INSERT INTO KhachHang (MaKhachHang, HoTen, CanCuocCongDan, SoDienThoai, Email, DiaChi, MaGiamGia) VALUES
(N'KH001', N'Hoàng Đức E', N'567890123456', N'0943210987', N'e.hoang@example.com', N'Hà Nội', N'GG001'),
(N'KH002', N'Nguyễn Thị F', N'678901234567', N'0932109876', N'f.nguyen@example.com', N'Đà Nẵng', N'GG002'),
(N'KH003', N'Phan Quang G', N'789012345678', N'0921098765', N'g.phan@example.com', N'Hồ Chí Minh', NULL),
(N'KH004', N'Đỗ Minh H', N'890123456789', N'0910987654', N'h.do@example.com', N'Cần Thơ', N'GG003');

-- Dữ liệu cho bảng Khách Hàng - Giảm Giá (nếu khách có nhiều voucher)
INSERT INTO KhachHang_GiamGia (MaKhachHang, MaGiamGia) VALUES
(N'KH001', N'GG001'),
(N'KH002', N'GG002'),
(N'KH004', N'GG003'),
(N'KH004', N'GG004');

-- Dữ liệu cho bảng Phòng
-- Dữ liệu cho bảng Phòng
INSERT INTO Phong (MaPhong, LoaiPhong, GiaPhong, TinhTrang, SoLuongPhong, HinhAnhPhong) VALUES
(N'P001', N'Standard', 500000, N'1', 5, N'https://i.pinimg.com/474x/0e/7b/89/0e7b89241b57c692d2f8c2fdf8cdfa6c.jpg'),
(N'P002', N'Deluxe', 700000, N'1', 3, N'https://i.pinimg.com/474x/96/b3/55/96b35558e25a803cfdf02706b5e9cf7c.jpg'),
(N'P003', N'Suite', 1500000, N'4', 2, N'https://i.pinimg.com/474x/6a/9e/c4/6a9ec45c4a02c97515b284fe807d7dd6.jpg'),
(N'P004', N'VIP', 2500000, N'2', 1, N'https://i.pinimg.com/474x/eb/9b/2f/eb9b2f0e79184fb6ed4d637ed3954607.jpg'),
(N'P005', N'Standard', 500000, N'1', 4, N'https://i.pinimg.com/474x/c3/4c/46/c34c464c7e68026d9ed9d219dd45b14f.jpg'),
(N'P006', N'Deluxe', 700000, N'3', 2, N'https://i.pinimg.com/474x/07/11/12/0711128af3646878419cdd5c4eee9e0a.jpg'),
(N'P007', N'Suite', 1500000, N'1', 1, N'https://i.pinimg.com/474x/35/06/44/3506442d8c59a3c0c1ab42a75b788cf7.jpg'),
(N'P008', N'VIP', 2500000, N'1', 2, N'https://i.pinimg.com/474x/18/27/ea/1827ea585709fae717ba4385e311f0bb.jpg');


-- Dữ liệu cho bảng Đặt Phòng
INSERT INTO DatPhong (MaDatPhong, MaKhachHang, MaNhanVien, NgayDat, NgayNhanPhong, NgayTraPhong, TrangThai) VALUES
(N'DP001', N'KH001', N'N001', '2025-06-01', '2025-06-05', '2025-06-10', N'1'),
(N'DP002', N'KH002', N'N002', '2025-06-02', '2025-06-06', '2025-06-11', N'2'),
(N'DP003', N'KH003', N'N003', '2025-06-03', '2025-06-07', '2025-06-12', N'1'),
(N'DP004', N'KH004', N'N004', '2025-06-04', '2025-06-08', '2025-06-13', N'1');

-- Dữ liệu cho bảng Đặt Phòng - Phòng
INSERT INTO DatPhong_Phong (MaDatPhong, MaPhong) VALUES
(N'DP001', N'P001'),
(N'DP002', N'P002'),
(N'DP003', N'P003'),
(N'DP004', N'P004');

-- Dữ liệu cho bảng Dịch Vụ
INSERT INTO DichVu (MaDichVu, TenDichVu, DonGia, HinhAnhDichVu) VALUES
(N'DV001', N'Giặt ủi', 100000, N'https://i.pinimg.com/474x/cc/b3/29/ccb32998122a6fc94d9b5fa55a2910fe.jpg'),
(N'DV002', N'Bữa sáng', 150000, N'https://i.pinimg.com/474x/41/cd/fe/41cdfeb6994c21f4ada49959b26e3c7a.jpg'),
(N'DV003', N'Massage', 300000, N'https://i.pinimg.com/736x/31/fb/81/31fb81b3e79c985f063381d7d3872d2f.jpg'),
(N'DV004', N'Xông hơi', 250000, N'https://i.pinimg.com/474x/79/f4/6d/79f46deec307caade132d5e0b3faded6.jpg'),
(N'DV005', N'Thức ăn nhanh', 80000, N'https://i.pinimg.com/474x/9c/12/a4/9c12a4146d459374bddd79e6ae557a99.jpg'),
(N'DV006', N'Đồ uống', 60000, N'https://i.pinimg.com/474x/20/60/0d/20600d922d5ded83a3325e256c613330.jpg'),
(N'DV007', N'Xe đưa đón', 500000, N'https://i.pinimg.com/474x/c9/0a/f3/c90af39c71aaa392d47c6c9549e5beb4.jpg');


-- Dữ liệu cho bảng Chi Tiết Dịch Vụ
INSERT INTO ChiTietDichVu (MaChiTietDichVu, MaDatPhong, MaDichVu, SoLuong, ThanhTien) VALUES
(N'CDV001', N'DP001', N'DV001', 2, 200000),
(N'CDV002', N'DP002', N'DV002', 1, 150000),
(N'CDV003', N'DP003', N'DV003', 1, 300000),
(N'CDV004', N'DP004', N'DV004', 3, 150000);

-- Dữ liệu cho bảng Hóa Đơn Thanh Toán
INSERT INTO HoaDonThanhToan (MaHoaDon, MaDatPhong, MaNhanVien, NgayLapHoaDon, TongTien, TrangThaiThanhToan, HinhThucThanhToan, MaGiamGia, GiamGiaApDung, VAT) VALUES
(N'HD001', N'DP001', N'N001', '2025-06-10', 1300000, N'1', 1, N'GG001', 100000, 10),
(N'HD002', N'DP002', N'N002', '2025-06-11', 1600000, N'0', 2, NULL, 0, 10),
(N'HD003', N'DP003', N'N003', '2025-06-12', 1900000, N'1', 1, NULL, 0, 10),
(N'HD004', N'DP004', N'N004', '2025-06-13', 2200000, N'1', 2, N'GG003', 200000, 10);

-- Thêm tài khoản nhân viên
INSERT INTO TaiKhoan (TenDangNhap, MatKhau, LoaiTaiKhoan, MaNhanVien, TrangThai) VALUES
('nguyenvana', 'password123', '2', 'N001', '1'),
('tranthib', 'password456', '2', 'N002', '1'),
('lehoangc', 'password789', '2', 'N003', '1'),
('phamminhd', 'password321', '2', 'N004', '1');

SELECT * FROM NhanVien;

-- Thêm tài khoản khách hàng
INSERT INTO TaiKhoan (TenDangNhap, MatKhau, LoaiTaiKhoan, MaKhachHang, TrangThai) VALUES
('hoangduce', 'pass567', '1', 'KH001', '1'),
('nguyenthif', 'pass678', '1', 'KH002', '1'),
('phanquangg', 'pass789', '1', 'KH003', '1'),
('dohminhh', 'pass890', '1', 'KH004', '1');



SELECT * FROM NhanVien;
SELECT * FROM KhachHang;



--############################################################################################
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--#############################################################################################



-- Kiểm tra lại dữ liệu đã thêm
SELECT * FROM KhachHang;

SELECT MaDatPhong FROM DatPhong;



------------------------------------------------------------------
ALTER TABLE DatPhong DROP CONSTRAINT FK__DatPhong__MaNhan__440B1D61;
ALTER TABLE HoaDonThanhToan DROP CONSTRAINT FK__HoaDonTha__MaNha__4CA06362;
ALTER TABLE Phong DROP CONSTRAINT FK__DatPhong___MaPho__5070F446;



DROP TABLE NhanVien;
DROP TABLE Phong;


---------------------------------------------------------------------------
SELECT 
    OBJECT_NAME(fk.parent_object_id) AS TableName,
    fk.name AS ForeignKeyName
FROM 
    sys.foreign_keys AS fk
WHERE 
    OBJECT_NAME(fk.referenced_object_id) = 'NhanVien';

select *from Phong
select *from DatPhong
select *from NhanVien
select *from KhachHang
select *from DichVu
select *from ChiTietDichVu
-----------------------------------------------------
SELECT * FROM KhachHang WHERE HoTen LIKE '%A%';
SELECT * FROM DichVu WHERE DonGia < 100000;
SELECT * FROM NhanVien ORDER BY HoTen ASC;
SELECT * FROM HoaDonThanhToan WHERE TrangThaiThanhToan = 1;






--############################################################################################
--////////////////////////////////////////////////////////////////////////////////////////////
--//////////////////////////							//////////////////////////////////////
--//////////////////////////   TRUY VẤN DỮ LIỆU SQL		//////////////////////////////////////
--//////////////////////////							//////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--#############################################################################################

-- 6. Lấy tất cả khách hàng
SELECT * FROM KhachHang;

-- 7. Lấy tất cả phòng đang trống
SELECT * FROM Phong WHERE TinhTrang = '1';

-- 8. Lấy danh sách đặt phòng trong tháng 3/2024
SELECT * FROM DatPhong WHERE MONTH(NgayNhanPhong) = 3 AND YEAR(NgayNhanPhong) = 2024;

-- 9. Lấy danh sách dịch vụ có giá trên 100.000 VNĐ
SELECT * FROM DichVu WHERE DonGia > 100000;

-- 10. Lấy thông tin hóa đơn chưa thanh toán
SELECT * FROM HoaDonThanhToan WHERE TrangThaiThanhToan = '0';


--##################### --Câu lệnh GROUP BY (Nhóm dữ liệu) #########################

-- 21. Đếm số lượng khách hàng theo từng thành phố
SELECT DiaChi, COUNT(*) AS SoLuongKhachHang FROM KhachHang GROUP BY DiaChi;

-- 22. Tổng doanh thu của từng loại phòng
SELECT LoaiPhong, SUM(GiaPhong) AS TongDoanhThu FROM Phong GROUP BY LoaiPhong;

-- 23. Đếm số lượng đặt phòng theo từng trạng thái
SELECT TrangThai, COUNT(*) AS SoLuong FROM DatPhong GROUP BY TrangThai;

-- 24. Tổng tiền hóa đơn theo từng phương thức thanh toán
SELECT HinhThucThanhToan, SUM(TongTien) AS TongDoanhThu FROM HoaDonThanhToan GROUP BY HinhThucThanhToan;

-- 25. Số lần sử dụng từng dịch vụ
SELECT MaDichVu, COUNT(*) AS SoLanSuDung FROM ChiTietDichVu GROUP BY MaDichVu;

--##################### 6. Câu lệnh ORDER BY (Sắp xếp dữ liệu) #########################

-- 26. Lấy danh sách khách hàng theo tên (A-Z)
SELECT * FROM KhachHang ORDER BY HoTen ASC;

-- 27. Lấy danh sách phòng theo giá giảm dần
SELECT * FROM Phong ORDER BY GiaPhong DESC;

-- 28. Lấy danh sách dịch vụ có giá cao nhất trước
SELECT * FROM DichVu ORDER BY DonGia DESC;

-- 29. Lấy danh sách hóa đơn sắp xếp theo ngày lập gần nhất
SELECT * FROM HoaDonThanhToan ORDER BY NgayLapHoaDon DESC;

-- 30. Lấy danh sách nhân viên theo vai trò (Lễ tân trước)
SELECT * FROM NhanVien ORDER BY VaiTro ASC;


--##################### 6. Câu lệnh ORDER BY (Sắp xếp dữ liệu) #########################

-- 36. Tìm khách hàng có tên bắt đầu bằng "Nguyễn"
SELECT * FROM KhachHang WHERE HoTen LIKE 'Trần%';

-- 37. Lấy danh sách phòng có giá từ 500,000 đến 1,500,000 VNĐ
SELECT * FROM Phong WHERE GiaPhong BETWEEN 500000 AND 1500000;

-- 38. Đếm số lượng khách hàng
SELECT COUNT(*) FROM KhachHang;

-- 39. Tính trung bình giá phòng
SELECT AVG(GiaPhong) FROM Phong;

-- 40. Tổng doanh thu từ hóa đơn đã thanh toán
SELECT SUM(TongTien) FROM HoaDonThanhToan WHERE TrangThaiThanhToan = '1';



SELECT *FROM KhachHang



--############################################################################################
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--////////////////////////////////////////////////////////////////////////////////////////////
--#############################################################################################
--*NOTE









