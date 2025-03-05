#include "main.h"

#include "stm32f1xx_hal.h"
#include <span>
#include <array>
#include <vector>

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


struct Packet {
    uint8_t n;      
    uint16_t freq;   
};

class Generator {
public:
	void add(std::span<const Packet> signal) {
	    dma_buffer.clear();
	    for (const auto& packet : signal) {
	        uint16_t arr = packet.freq - 1; 
	        uint16_t ccr = arr / 2; 
	        uint16_t rcr = packet.n ; 
	        dma_buffer.emplace_back(arr); // ARR
	        dma_buffer.emplace_back(rcr); // RCR
	        dma_buffer.emplace_back(ccr); // CCR1
	    }
	}

    void start() {
        Configure_TIM1();
        Configure_DMA();
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    }

private:
    std::vector<uint16_t> dma_buffer; 
    TIM_HandleTypeDef htim1;
    DMA_HandleTypeDef hdma_tim1_up;

    void Configure_DMA() {
        __HAL_RCC_DMA1_CLK_ENABLE();

        hdma_tim1_up.Instance = DMA1_Channel5;
        hdma_tim1_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_tim1_up.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_tim1_up.Init.MemInc = DMA_MINC_ENABLE;
        hdma_tim1_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim1_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_tim1_up.Init.Mode =  DMA_CIRCULAR;
        hdma_tim1_up.Init.Priority = DMA_PRIORITY_HIGH;

        HAL_DMA_Init(&hdma_tim1_up);
        __HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_UPDATE], hdma_tim1_up);
 
        HAL_TIM_DMABurst_MultiWriteStart(&htim1, TIM_DMABASE_ARR, TIM_DMA_UPDATE,
        		(uint32_t*)dma_buffer.data(), TIM_DMABURSTLENGTH_3TRANSFERS,
				dma_buffer.size());
    }

    void Configure_TIM1() {
        __HAL_RCC_TIM1_CLK_ENABLE();

        htim1.Instance = TIM1;
        htim1.Init.Prescaler = TIM_PSC;
        htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim1.Init.Period = dma_buffer[0]; // Первоначальная настройка частоты
        htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim1.Init.RepetitionCounter = dma_buffer[1];

        HAL_TIM_Base_Init(&htim1);

        TIM_OC_InitTypeDef sConfigOC = {0};
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = dma_buffer[2]; // Скважность 50%
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;//TIM_OCFAST_DISABLE;

        HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

        __HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);

    }

    static constexpr uint16_t TIM_PSC = 72 ;
};

int main(void)
{
  HAL_Init();
  
  SystemClock_Config();

  Generator generator;
     std::array<Packet, 3> signal{{{1, 2000}, {2, 1000}, {1, 500}}};
     generator.add(signal);
     generator.start();
 
  while (1)
  {
  
  }

}


