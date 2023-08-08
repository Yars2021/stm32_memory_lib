//
// Created by Dizarker on 16.03.2021.
//

#include "bootloader.h"

static bootloader_info_t bInfo;

#if F1_CHECK
/*!
 * \brief Функция поиска номера номера страницы по адресу во FLASH.
 */
static int get_page_num_f1 (uint32_t fw_address)
{
    // Если адрес меньше начального адреса FLASH - возвращаем ошибку
    if (fw_address < BOOTLOADER_FLASH_START_ADDR) return -1;

    // Уберем начальное смещение адреса
    int start = (int)fw_address - BOOTLOADER_FLASH_START_ADDR;
    int res = start / BOOTLOADER_F1_PAGE_SIZE;

    if (res > BOOTLOADER_F1_TOTAL_PAGES_NUMBER) return -1;
    else return res;
}
#elif F4_CHECK
/*!
 * \brief Таблица размеров секторов памяти. Начиная с пятого, все остальные сектора одного размера
 */
static const int sectors_size[] = {
        BOOTLOADER_F4_SECTOR0_SIZE,
        BOOTLOADER_F4_SECTOR1_SIZE,
        BOOTLOADER_F4_SECTOR2_SIZE,
        BOOTLOADER_F4_SECTOR3_SIZE,
        BOOTLOADER_F4_SECTOR4_SIZE,
        BOOTLOADER_F4_SECTOR5_AND_OTHER_SIZE
};

/*!
 * \brief Функция поиска номера номера сектора по адресу во FLASH.
 */
static int get_sector_num_f4 (uint32_t fw_address)
{
    // Если адрес меньше начального адреса FLASH - возвращаем ошибку
    if (fw_address < BOOTLOADER_FLASH_START_ADDR) return -1;

    // Уберем начальное смещение адреса
    int start = (int)fw_address - BOOTLOADER_FLASH_START_ADDR;

    int i;

    for (i = 0; i <= BOOTLOADER_F4_TOTAL_SECTORS_NUMBER; i++)
    {
        start -= i <= BOOTLOADER_F4_SECTORSIZE_SAT ? sectors_size[i]-1 : sectors_size[BOOTLOADER_F4_SECTORSIZE_SAT]-1;
        if (start < 0) {
            break;
        }
        start--;
    }

    // Если указан адрес, который за пределами наших возможностей
    if (i > BOOTLOADER_F4_TOTAL_SECTORS_NUMBER) return -1;
    else return i;
}
#endif

/*!
 * \brief Функция стирания старницы/секторов памяти.
 * \param[in] int start - номер сектора или страницы памяти, с которой начнется затирание
 * \param[in] int count - количество страниц/секторов памяти подлежащих затиранию
 */
static void flash_erase (int start, int count)
{
    uint32_t sErr;
    FLASH_EraseInitTypeDef sErase = {
#if F1_CHECK
            .Banks = 0,
            .TypeErase = FLASH_TYPEERASE_PAGES,
            .NbPages = count,
            .PageAddress = start * BOOTLOADER_F1_PAGE_SIZE + BOOTLOADER_FLASH_START_ADDR
#elif F4_CHECK
            .TypeErase = FLASH_TYPEERASE_SECTORS,
            .Banks = 0,
            .VoltageRange = FLASH_VOLTAGE_RANGE_3,
            .Sector = start,
            .NbSectors = count
#endif
    };
    HAL_FLASHEx_Erase(&sErase, &sErr);
}

/*!
 * \brief Функция проверки - есть ли прошивка приложения по адресу ее начала.
 * \details Любая прошивка начинается с таблицы векторов прерываний, поэтому если прошивка есть, то первый байт
 * никогда не равен 0xff, а принимает это значение только при стирании. Поэтому можно принять точное решение -
 * если байт равен 0xff - то страница(сектор) памяти затерт и прошивки нет.
 * \return возвращает результат проверки на наличие прошивки.
 * \retval[0] - прошивки нет
 * \retval[1] - прошивка есть
 */
int bootloader_check_application (void)
{
    int32_t testWord;

    testWord = *(__IO uint32_t*)BOOTLOADER_APPLICATION_ADDRESS;

    if (testWord != -1)
    {
        // Прошивка по адресу есть
        return 1;
    }
    else
    {
        // Прошивки по адресу нет
        return 0;
    }
}

/*!
 * \brief Функция перехода из бутлоадера в приложение.
 * \details В функции происходит отключение тактования всей перефирии, поэтому приложение стартует, фактически, с чистого листа.
 * \warning Программа приложения должна первым делом сместить адрес стека на свой адрес и указать новый адрес таблицы векторов прерываний.
 */
void bootloader_run_application (void)
{
    pFunction Jump_to_application;

    __disable_irq();

    // Необходимо вырубить всю перефирию перед переходом
    HAL_DeInit();

    Jump_to_application = (pFunction)(*(uint32_t*)(BOOTLOADER_APPLICATION_ADDRESS + 4));
    __set_MSP(*(uint32_t*)BOOTLOADER_APPLICATION_ADDRESS);
    Jump_to_application();
}
/*!
 * \brief Функция подготовки к записи прошивки.
 * \details Функция производит разблокировку flash памяти и ее очистку под прошивку.
 * \param[in] size_t fwsize - размер прошивки в байтах
 * \return - возвращает флаг ошибки
 * \retval BOOTLOADER_NO_ERROR - успешное выполнение, нет ошибок
 * \retval BOOTLOADER_APPADDR_INVALID - неправильно указан адрес начала прошивки приложения
 * \retval BOOTLOADER_FWSIZE_INVALID - прошивка не помещается в отведенное количество памяти
 * \retval BOOTLOADER_UNLOCK_ERROR - не получается снять блокировку на запись с flash
 */
bootloader_error_t bootloader_prepare_for_burn (size_t fwsize)
{
    // Считаем, сколько надо страниц/секторов затереть. И поместится ли вообще к нам прошивка
#ifdef F1_CHECK
    bInfo.start = get_page_num_f1(BOOTLOADER_APPLICATION_ADDRESS);
#elif F4_CHECK
    bInfo.start = get_sector_num_f4(BOOTLOADER_APPLICATION_ADDRESS);
#endif
    if (bInfo.start <= 0)
    {
        // Адрес аппликухи неверный или совпадает с адресом бутлоадера
        return BOOTLOADER_APPADDR_INVALID;
    }

#ifdef F1_CHECK
    bInfo.stop = get_page_num_f1(BOOTLOADER_APPLICATION_ADDRESS + fwsize);
#elif F4_CHECK
    bInfo.stop = get_sector_num_f4(BOOTLOADER_APPLICATION_ADDRESS + fwsize);
#endif
    {
        // Прошивка не вмещается во флэш
        if (bInfo.stop < 0) return BOOTLOADER_FWSIZE_INVALID;
    }

    // Снимаем блокировку записи flash
    if (HAL_FLASH_Unlock() != HAL_OK)
    {
        return BOOTLOADER_UNLOCK_ERROR;
    }

    // Затираем нужные сектора
    flash_erase (bInfo.start, bInfo.stop - bInfo.start + 1);

    // Инициализация crc, если вдруг еще не сделана
    crc32hw_init();
    bInfo.fwsize = fwsize;

    // Сбросим все флаги ошибок
#if F1_CHECK
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR);
#elif F4_CHECK
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
#endif

    return BOOTLOADER_NO_ERROR;
}

/*!
 * \brief Функция непосредственно записи данных прошивки во flash память
 * \details Функция принимает данные прошивки пакетами и записывает этот пакет данных во flash память по указанному смещению
 * относительно адреса начала прошивки приложения.
 * \details Если во время записи произойдет ошибка - первая страница/первый сектор прошивки приложения будет затерт, а Flash память заблокируется
 * \param[in] uint32_t offset - смещение пакета относительно начального адреса прошивки приложения
 * \param[in] uint8_t *data - указатель на буфер, где лежат данные прошивки
 * \param[in] size_t size - размер данных в данном пакете
 * \return возвращает флаг ошибки
 * \retval BOOTLOADER_NO_ERROR - успешное выполнение, нет ошибок
 * \retval BOOTLOADER_FLASH_WRITE_ERROR - ошибка во время записи в память
 */
bootloader_error_t bootloader_write_data (uint32_t offset, uint8_t *data, size_t size)
{
    HAL_StatusTypeDef res;

    for (size_t k = 0; k < size; k += sizeof(uint32_t))
    {
        res = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, BOOTLOADER_APPLICATION_ADDRESS + offset + k, (uint64_t)*((uint32_t*)(data + k)));
        if (res != HAL_OK)
        {
            // Ошибка записи. Останавливаем процесс и стираем обратно первую страницу/сектор, чтобы бутлоадер при перезагрузке мог понять, что прошивки нет
            flash_erase(bInfo.start, 1);
            HAL_FLASH_Lock();
            return BOOTOLOADER_FLASH_WRITE_ERROR;
        }
    }
    return BOOTLOADER_NO_ERROR;
}

/*!
 * \brief Функция проверки на правильность
 * \details Производится проверка соответствии контрольных сумм, между прошивками - эталонный образец,
 * который был посчитан перед передачей и той, которая была записана во flash память. Если контрольные суммы совпали, то
 * прошивка считается успешной. Если суммы не совпадают - возвращается соответствующая ошибка и первая страница(сектор)
 * прошивки приложения затирается
 * \param[in] uint32_t crcExpected - эталонная контрольная сумма
 * \return флаг ошибки выполнения
 * \retval[BOOTLOADER_NO_ERROR] - нет ошибок, прошивка успешно завершена
 * \retval[BOOTLOADER_CRC_ERROR] - контрольные суммы не совпали
 */
bootloader_error_t bootloader_finalize (uint32_t crcExpected)
{
    bootloader_error_t  res;
    uint32_t crcCalculated;

    // Блокируем flash на случай случайной перезаписи
    HAL_FLASH_Lock();

    // Посчитаем CRC полученной прошивки
    crcCalculated = crc32hw((uint8_t*)BOOTLOADER_APPLICATION_ADDRESS, bInfo.fwsize, 1);

    if (crcCalculated != crcExpected)
    {
        // CRC не совпало. Выкидываем соответствующую ошибку и стираем первую страницу/сектор
        flash_erase(bInfo.start, 1);
        res = BOOTLOADER_CRC_ERROR;
    }
    else
    {
        res = BOOTLOADER_NO_ERROR;
    }

    return res;
}
