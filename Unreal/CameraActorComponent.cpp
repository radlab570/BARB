// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraActorComponent.h"
#include "Engine/GameEngine.h"
#include "Async/Async.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "IPAddress.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Constants
constexpr int32 UDP_PORT = 10000; // Port number
constexpr int32 NUM_FLOATS = 7; // Number of floats in the received data (3 Translation + 4 for Quaternions)
constexpr bool DEBUG = false; // Debug flag
constexpr int32 BUFFER_SIZE = NUM_FLOATS * sizeof(float); // Buffer size for incoming packets

float quatW;

// Sets default values for this component's properties
UCameraActorComponent::UCameraActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UdpSocket = nullptr;
	quatW = 0.0f;
}


// Called when the game starts
void UCameraActorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && DEBUG)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Hello World!"));
	}

	// Ensure the owner is set to movable
	AActor* Owner = GetOwner();
	if (Owner)
	{
		UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(Owner->GetRootComponent());
		if (RootComponent)
		{
			RootComponent->SetMobility(EComponentMobility::Movable);
		}
	}

	// Create and configure the UDP socket
	UdpSocket = FUdpSocketBuilder(TEXT("CameraActorSocket"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToPort(UDP_PORT)
		.WithReceiveBufferSize(BUFFER_SIZE);

	if (UdpSocket)
	{
		if (GEngine && DEBUG)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("UDP Listener started successfully."));
		}
	}
	else
	{
		if (GEngine && DEBUG)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to start UDP Listener."));
		}
	}
}


// Called every frame
void UCameraActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!UdpSocket) return;

	// Poll the socket for incoming data
	uint32 Size;
	while (UdpSocket->HasPendingData(Size))
	{
		if (Size >= BUFFER_SIZE)
		{
			TArray<uint8> ReceivedData;
			ReceivedData.SetNumUninitialized(BUFFER_SIZE);

			int32 BytesRead;
			if (UdpSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesRead))
			{
				// Convert the received data to an array of floats
				float* FloatData = reinterpret_cast<float*>(ReceivedData.GetData());
				FVector Position(-FloatData[2] * 200, FloatData[0] * 200, FloatData[1] * 200 + 100); // First 3 floats are position
				FQuat Rotation(FloatData[5], -FloatData[3], -FloatData[4], FloatData[6]); // Last 4 floats are quaternion

				// Convert quaternion to rotator (pitch, yaw, roll)
				FRotator Rotator = Rotation.Rotator();

				// Get the player character
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					APawn* PlayerPawn = PlayerController->GetPawn();
					if (PlayerPawn)
					{
						// Set the player's position and rotation
						PlayerPawn->SetActorLocationAndRotation(Position, Rotation);

						// Update the player controller's control rotation
						PlayerController->SetControlRotation(Rotator);

						// Debug: Display the new position and rotation
						if (GEngine && DEBUG)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
								FString::Printf(TEXT("Player Position: X=%f, Y=%f, Z=%f"), Position.X, Position.Y, Position.Z));
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
								FString::Printf(TEXT("Player Rotation: X=%f, Y=%f, Z=%f, W=%f"), Rotation.X, Rotation.Y, Rotation.Z, Rotation.W));
						}
					}
				}
			}
		}
	}
}

// Called when the game ends
void UCameraActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UdpSocket)
	{
		UdpSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(UdpSocket);
		UdpSocket = nullptr;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("UDP Listener Stopped."));
		}
	}
}

